#include "nsga2_solver.h"

NSGA2Solver::NSGA2Solver(const std::vector<OptimizationProblem> &problems, size_t populationSize) : Solver(problems[0], populationSize)
{
    always_assert(problems.size() > 1);

    m_objectiveCount = problems.size();
    m_objectiveFunctions.resize(m_objectiveCount);
    for (size_t i = 0; i < m_objectiveCount; i++)
    {
        m_objectiveFunctions[i] = problems[i].testFunction;
    }
    m_randomIndividual = std::uniform_int_distribution<size_t>(0, m_populationSize - 1);
    m_randomDim = std::uniform_int_distribution<size_t>(0, m_dimensionCount - 1);
}

void NSGA2Solver::initialize_population()
{
    using namespace azgra::collection;
    const auto individuals = generate_random_individuals();
    m_population = select(individuals.begin(), individuals.end(), [this](const Individual &individual) {
        std::vector<f64> evaluations(this->m_objectiveCount);
        for (size_t i = 0; i < this->m_objectiveCount; i++)
        {
            evaluations[i] = m_objectiveFunctions[i](individual.attributes);
        }
        return NSGA2Individual(individual, evaluations);
    });
}

bool NSGA2Solver::dominates(const NSGA2Individual &p, const NSGA2Individual &q) const
{
    for (size_t i = 0; i < m_objectiveCount; i++)
    {
        if (p.evaluations[i] >= q.evaluations[i])
        {
            return false;
        }
    }
    return true;
}

std::vector<std::vector<size_t>> NSGA2Solver::fast_non_dominated_sort(std::vector<NSGA2Individual> &population)
{
    std::vector<std::vector<size_t>> fronts;

    // Initial sort
    std::vector<size_t> firstFront;
    for (size_t pIndex = 0; pIndex < population.size(); pIndex++)
    {
        NSGA2Individual &p = population[pIndex];
        size_t Np = 0;
        std::vector<size_t> Sp;

        for (size_t qIndex = 0; qIndex < population.size(); qIndex++)
        {
            if (pIndex == qIndex)
            {
                continue;
            }
            NSGA2Individual &q = population[qIndex];
            if (dominates(p, q))
            {
                Sp.push_back(qIndex);
            }
            else if (dominates(q, p))
            {
                Np += 1;
            }
        }
        p.dominatedByCount = Np;
        p.dominatedSolutions = std::move(Sp);
        if (Np == 0)
        {
            p.rank = 0;
            firstFront.push_back(pIndex);
        }
    }
    fronts.push_back(firstFront);

    size_t i = 0;
    while (!fronts[i].empty())
    {
        std::vector<size_t> Q;
        for (const size_t pIndex : fronts[i])
        {
            for (const size_t dominatedIndex : population[pIndex].dominatedSolutions)
            {
                NSGA2Individual &q = population[dominatedIndex];
                q.dominatedByCount -= 1;
                if (q.dominatedByCount == 0)
                {
                    q.rank = i + 1;
                    Q.push_back(dominatedIndex);
                }
            }
        }
        ++i;
        fronts.push_back(Q);
    }
    return fronts;
}

size_t NSGA2Solver::tournament_selection(const size_t parentA, bool useCrowdedComparsionOp, const size_t k)
{
    size_t bestIndex = 0;
    size_t bestRank = 9999;
    float bestDist = 0.0f;

    for (size_t i = 0; i < k; i++)
    {
        size_t randomIndex = m_randomIndividual(m_mt);
        while(randomIndex == parentA)
        {
            randomIndex = m_randomIndividual(m_mt);
        }

        const NSGA2Individual &rndParent = m_population[randomIndex];

        if ((rndParent.rank < bestRank) || ((rndParent.rank == bestRank) && ( rndParent.distance > bestDist)))
        {
            bestRank = rndParent.rank;
            bestDist = rndParent.distance;
            bestIndex = randomIndex;
        }
    }
    return bestIndex;
}

NSGA2Individual NSGA2Solver::crossover_with_mutation(const NSGA2Individual &parentA, const NSGA2Individual &parentB)
{

    always_assert(m_dimensionCount == 2);
    std::vector<f64> attributes(m_dimensionCount);
    std::vector<f64> fnEvals(m_dimensionCount);
    if (m_dimensionCount == 2)
    {
        if (m_rand01(m_mt))
        {
            attributes[0] = parentA.attributes[0];
            attributes[1] = parentB.attributes[1];
        }
        else
        {
            attributes[0] = parentB.attributes[0];
            attributes[1] = parentA.attributes[1];
        }
    }
    else
    {
        // TODO.
    }

    // Mutation
    if (m_rand01(m_mt) < 0.2)
    {
        attributes[m_randomDim(m_mt)] *= m_rand01(m_mt);
    }

    for (size_t i = 0; i < m_objectiveCount; i++)
    {
        fnEvals[i] = m_objectiveFunctions[i](attributes);
    }

    return NSGA2Individual(attributes, fnEvals);
}

std::vector<NSGA2Individual> NSGA2Solver::create_offsprings(bool useCrowdedComparsionOp)
{
    // Assign distance to population members.
    crowded_sort(m_population);
    // Assign rank to parents, used in tournament selection.
    auto parentFronts = fast_non_dominated_sort(m_population);

    std::vector<NSGA2Individual> offsprings(m_populationSize);

    for (size_t parentAIndex = 0; parentAIndex < m_populationSize; parentAIndex++)
    {
        // Choose second parent in tournament selection
        size_t secondParentIndex = tournament_selection(parentAIndex, useCrowdedComparsionOp);
        const NSGA2Individual &parentA = m_population[parentAIndex];
        const NSGA2Individual &parentB = m_population[secondParentIndex];
        // Create offspring.
        offsprings[parentAIndex] = crossover_with_mutation(parentA, parentB);
    }

    return offsprings;
}

std::vector<std::pair<float,float>> NSGA2Solver::solve()
{
    
    initialize_population();

    for (size_t it = 0; it < m_iterationCount; it++)
    {
        const auto offsprings = create_offsprings(it != 0);

        // Create combined population
        std::vector<NSGA2Individual> combinedPopulation;
        combinedPopulation.insert(combinedPopulation.end(), m_population.begin(), m_population.end());
        combinedPopulation.insert(combinedPopulation.end(), offsprings.begin(), offsprings.end());

        // Combined population fronts
        const auto fronts = fast_non_dominated_sort(combinedPopulation);

        size_t nextPopIndex = 0;
        std::vector<NSGA2Individual> nextPopulation(m_populationSize);
        long remaining = m_populationSize;
        size_t frontIndex = 0;
        while (remaining > 0)
        {
            const size_t frontSize = fronts[frontIndex].size();
            if (frontSize < remaining)
            {
                // All from frontIndex front to next population
                for (size_t i = 0; i < frontSize; i++)
                {
                    nextPopulation[nextPopIndex++] = combinedPopulation[fronts[frontIndex][i]];
                }
            }
            else
            {
                // Crowding distance sorting.
                std::vector<NSGA2Individual> remainingIndividuals(frontSize);
                for (size_t i = 0; i < frontSize; i++)
                {
                    remainingIndividuals[i] = combinedPopulation[fronts[frontIndex][i]];
                }

                crowded_sort(remainingIndividuals);

                for (size_t i = 0; i < remaining; i++)
                {
                    nextPopulation[nextPopIndex++] = remainingIndividuals[i];
                }
            }
            remaining -= frontSize;
            ++frontIndex;
        }

        m_population = nextPopulation;
    }
    std::vector<std::pair<float,float>> result(m_populationSize);
    size_t i = 0;
    for (const auto &individual : m_population)
    {
        result[i++] = std::make_pair(individual.evaluations[0],individual.evaluations[1]);
    }
    return result;
}

void NSGA2Solver::crowded_sort(std::vector<NSGA2Individual> &individuals)
{
    for (NSGA2Individual &indiv : individuals)
    {
        indiv.distance = 0.0f;
    }
    const size_t size = individuals.size();

    for (size_t objIndex = 0; objIndex < m_objectiveCount; objIndex++)
    {
        float objMax = std::numeric_limits<float>::min();
        float objMin = std::numeric_limits<float>::max();

        for (NSGA2Individual &indiv : individuals)
        {
            indiv.compareOnIndex = objIndex;
            const float evalValue = indiv.evaluations[objIndex];
            if (evalValue > objMax)
            {
                objMax = evalValue;
            }
            if (evalValue < objMin)
            {
                objMin = evalValue;
            }
        }

        std::sort(individuals.begin(), individuals.end());

        individuals[0].distance = INFINITY;
        individuals[size - 1].distance = INFINITY;

        for (size_t i = 1; i < (size - 1); i++)
        {
            individuals[i].distance += (individuals[i + 1].evaluations[objIndex] - individuals[i - 1].evaluations[objIndex]) / (objMax - objMin);
        }
    }

    // Descending distance
    std::sort(individuals.begin(), individuals.end(), std::greater<>());
}