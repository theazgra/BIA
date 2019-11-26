#include "FireflySolver.h"

FireflySolver::FireflySolver(const OptimalizationProblem &problem,
                             const size_t populationSize,
                             const f64 beta0,
                             const f64 alpha,
                             const f64 gamma) : Solver(problem, populationSize)
{
    m_beta0 = beta0;
    m_alpha = alpha;
    m_gamma = gamma;
}

void FireflySolver::initialize_fireflies()
{
    auto individuals = generate_random_individuals();
    m_fireflies.resize(m_populationSize);
    for (size_t i = 0; i < m_populationSize; ++i)
    {
        m_fireflies[i] = Firefly(individuals[i]);
    }
}

void FireflySolver::move_firefly(Firefly &current, const Firefly &other)
{
    const f64 r = current.distance(other);
    for (size_t dim = 0; dim < m_dimensionCount; ++dim)
    {
        current.attributes[dim] = current.attributes[dim] +
                                  (m_beta0 * (1.0 / (r + m_gamma)) * (other.attributes[dim] - current.attributes[dim])) +
                                  (m_alpha * (m_rand01(m_mt) - 0.5));
    }
}


void FireflySolver::move_best_firefly(Firefly &best)
{
    for (size_t dim = 0; dim < m_dimensionCount; ++dim)
    {
        best.attributes[dim] = best.attributes[dim] + (m_alpha * (m_rand01(m_mt) - 0.5));
    }
}


OptimizationResult FireflySolver::solve()
{
    OptimizationResult result = {};
    result.invidualsInTime.resize(m_iterationCount + 1);
    initialize_fireflies();
    set_best_firefly();

    for (size_t it = 0; it < m_iterationCount; ++it)
    {
        result.invidualsInTime[it] = azgra::collection::select(
                m_fireflies.begin(), m_fireflies.end(),
                [](const Firefly &ff)
                { return ff.attributes; });

        for (size_t i = 0; i < m_populationSize; ++i)
        {
            Firefly &currentFirefly = m_fireflies[i];
            if (currentFirefly.isBest)
            {
                //move_best_firefly(currentFirefly);
                continue;
            }
            for (size_t j = 0; j < m_populationSize; ++j)
            {
                if (i == j)
                {
                    continue;
                }

                Firefly &otherFirefly = m_fireflies[j];
                const f64 fitnessDelta = currentFirefly.fitness - otherFirefly.fitness;
                //if (otherFirefly.fitness < currentFirefly.fitness)
                // NOTE(Moravec): By moving only to the Fireflies which are better be some threshold we improve the performance.
                if (fitnessDelta > 1.0)
                {
                    move_firefly(currentFirefly, otherFirefly);
                }
            }
            currentFirefly.fitness = m_fitnessFunction(currentFirefly.attributes);
        }

        result.invidualsInTime[m_iterationCount] = azgra::collection::select(
                m_fireflies.begin(), m_fireflies.end(),
                [](const Firefly &ff)
                { return ff.attributes; });

        set_best_firefly();
        const auto &leader = get_best_firefly();
        f64 avg = average_fitness();
        fprintf(stdout, "Leader firefly cost: %.5f Average fitness: %.5f\n", leader.fitness, avg);
    }
    result.result = get_best_firefly().fitness;
    return result;
}

void FireflySolver::set_best_firefly()
{

    size_t bestIndex = 0;
    f64 bestFitness = std::numeric_limits<f64>::max();
    for (auto &firefly : m_fireflies)
    {
        firefly.isBest = false;
    }

    for (size_t i = 0; i < m_populationSize; ++i)
    {
        if (m_fireflies[i].fitness < bestFitness)
        {
            bestFitness = m_fireflies[i].fitness;
            bestIndex = i;
        }
    }
    m_fireflies[bestIndex].isBest = true;
}

Firefly const &FireflySolver::get_best_firefly() const
{
    for (const auto &firefly : m_fireflies)
    {
        if (firefly.isBest)
        {
            return firefly;
        }
    }
    always_assert(false && "Not found best firefly.");
}

f64 FireflySolver::average_fitness() const
{
    const f64 sum = azgra::collection::sum(m_fireflies.begin(), m_fireflies.end(), [](const Firefly &f)
    { return f.fitness; });
    const f64 avgFit = sum / static_cast<f64>(m_populationSize);
    return avgFit;
}




