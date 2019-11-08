#include "SOMASolver.h"


SOMASolver::SOMASolver(const optimalization::OptimalizationProblem &problem, const size_t populationSize, const size_t migrationCount)
{
    m_fitnessFunction = problem.testFunction;
    m_dimension = problem.dimensionCount;
    m_dimensionLimits = problem.dimensionLimits;
    m_populationSize = populationSize;
    m_migrations = migrationCount;

    m_generator = std::mt19937(m_rd());
    m_01rand = std::uniform_real_distribution<f64>(0.0, 1.0);
}

void SOMASolver::edit_parameters(const f64 pathLen, const f64 step, const f64 prt, const f64 minDiv)
{
    m_pathLen = pathLen;
    m_step = step;
    m_prt = prt;
    m_minDiv = minDiv;
}

std::vector<f64> SOMASolver::generate_perturbation_vector()
{
    std::vector<f64> prtVector(m_dimension);

    for (size_t dim = 0; dim < m_dimension; ++dim)
    {
        prtVector[dim] = (m_01rand(m_generator) < m_prt) ? 1.0 : 0.0;
    }

    return prtVector;
}

void SOMASolver::initialize_population()
{
    m_currentPopulation.resize(m_populationSize);
    for (size_t pop = 0; pop < m_populationSize; ++pop)
    {
        m_currentPopulation[pop] = generate_random_individual();
    }
}

Individual SOMASolver::generate_random_individual()
{
    Individual individual = {};
    individual.attributes.resize(m_dimension);
    for (size_t dim = 0; dim < m_dimension; ++dim)
    {
        const f64 dimMin = m_dimensionLimits[dim].min;
        const f64 dimMax = m_dimensionLimits[dim].max;
        individual.attributes[dim] = dimMin + (m_01rand(m_generator) * (dimMax - dimMin));
    }
    return individual;
}

SOMAResult SOMASolver::solve()
{
    SOMAResult result = {};
    result.invidualsInTime.resize(m_migrations);
    //TODO (Moravec): Record population history!.
    initialize_population();
    calculate_fitness_for_population(m_currentPopulation);
    f64 avgFitness = average_fitness();
    fprintf(stdout, "Initial average cost: %.5f\n", avgFitness);
    Individual leader = find_leader(m_currentPopulation);
    for (size_t migration = 0; migration < m_migrations; ++migration)
    {
        result.invidualsInTime[migration] = azgra::collection::select(m_currentPopulation.begin(), m_currentPopulation.end(), [](const
        Individual &individual)
        { return individual.attributes; });


        std::vector<Individual> nextPopulation(m_populationSize);

        for (size_t indiv = 0; indiv < m_populationSize; ++indiv)
        {
            std::vector<Individual> jumps;
            jumps.reserve((m_pathLen / m_step) + 1);
            f64 t = m_step;
            while (t <= m_pathLen)
            {
                auto perturbationVector = generate_perturbation_vector();
                jumps.push_back(migrate(m_currentPopulation[indiv], leader, perturbationVector, t));
                t += m_step;
            }
            calculate_fitness_for_population(jumps);
            Individual bestJump = find_leader(jumps);


            nextPopulation[indiv] = (bestJump.fitness <= m_currentPopulation[indiv].fitness) ? bestJump : m_currentPopulation[indiv];
        }

        m_currentPopulation = nextPopulation;
        leader = find_leader(m_currentPopulation);
        avgFitness = average_fitness();
        //fprintf(stdout, "\rMigration %lu/%lu average cost: %.5f Leader cost: %.5f", migration + 1, m_migrations, avgFitness, leader.fitness);
        fprintf(stdout, "Migration %lu/%lu average cost: %.5f Leader cost: %.5f\n", migration + 1, m_migrations, avgFitness, leader
        .fitness);
    }
    fprintf(stdout, "\rMigration %lu/%lu average cost: %.5f Leader cost: %.5f", m_migrations, m_migrations, avgFitness, leader.fitness);
    result.result = leader.fitness;
    fprintf(stdout, "\n");
    return result;
}

void SOMASolver::calculate_fitness_for_population(std::vector<Individual> &population)
{
    for (size_t pop = 0; pop < population.size(); ++pop)
    {
        population[pop].fitness = m_fitnessFunction(population[pop].attributes);
    }
}

Individual SOMASolver::find_leader(const std::vector<Individual> &population)
{
    f64 min = std::numeric_limits<f64>::max();
    Individual leader = {};
    for (size_t pop = 0; pop < population.size(); ++pop)
    {
        if (population[pop].fitness <= min)
        {
            leader = population[pop];
            min = leader.fitness;
        }
    }
    return leader;
}

Individual SOMASolver::migrate(const Individual &current, const Individual &leader, const std::vector<f64> &prt, const f64 t)
{
    Individual individual = {};
    individual.attributes.resize(m_dimension);
    for (size_t dim = 0; dim < m_dimension; ++dim)
    {
        individual.attributes[dim] = current.attributes[dim] + ((leader.attributes[dim] - current.attributes[dim]) * t * prt[dim]);
    }

    individual.fitness = m_fitnessFunction(individual.attributes);
    return individual;
}

f64 SOMASolver::average_fitness() const
{
    f64 result = 0.0;

    for (size_t indiv = 0; indiv < m_populationSize; ++indiv)
    {
        result += m_currentPopulation[indiv].fitness;
    }

    result /= static_cast<f64>(m_populationSize);
    return result;
}
