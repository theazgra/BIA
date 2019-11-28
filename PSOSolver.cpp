#include "PSOSolver.h"

PSOSolver::PSOSolver(const OptimalizationProblem &problem, const size_t particleCount, const size_t iterationCount)
{
    m_dimension = problem.dimensionCount;
    m_dimensionLimits = problem.dimensionLimits;
    m_fitnessFunction = problem.testFunction;
    m_iterationCount = iterationCount;
    m_particleCount = particleCount;

    m_generator = std::mt19937(m_rd());
    m_01rand = std::uniform_real_distribution<f64>(0.0, 1.0);

    m_randomAttribute.resize(m_dimension);
    m_randomVelocity.resize(m_dimension);
    m_vMax.resize(m_dimension);
    for (size_t dim = 0; dim < m_dimension; ++dim)
    {
        const f64 dimMin = m_dimensionLimits[dim].min;
        const f64 dimMax = m_dimensionLimits[dim].max;
        const f64 dimensionSize = dimMin < 0 ? (dimMax + abs(dimMin)) : dimMax - dimMin;
        m_randomAttribute[dim] = std::uniform_real_distribution<f64>(dimMin, dimMax);
        const f64 vMax = dimensionSize / 20.0;
        m_vMax[dim] = vMax;
        m_randomVelocity[dim] = std::uniform_real_distribution<f64>(-vMax, vMax);
    }
}

void PSOSolver::init_gBest(const std::vector<PSOParticle> &particles)
{
    f64 min = std::numeric_limits<f64>::max();
    for (const auto &particle : particles)
    {
        if (particle.fitness < min)
        {
            m_gBest = particle;
            min = m_gBest.fitness;
        }
    }
}

PSOParticle PSOSolver::generate_random_particle()
{
    PSOParticle particle = {};
    particle.attributes.resize(m_dimension);
    particle.velocity.resize(m_dimension);

    for (size_t dim = 0; dim < m_dimension; ++dim)
    {
        particle.attributes[dim] = m_dimensionLimits[dim].min +
                                   (m_01rand(m_generator) * (m_dimensionLimits[dim].max - m_dimensionLimits[dim].min));

        assert(particle.attributes[dim] >= m_dimensionLimits[dim].min && particle.attributes[dim] <= m_dimensionLimits[dim].max);

        particle.velocity[dim] = m_randomVelocity[dim](m_generator);// 0;
        particle.fitness = m_fitnessFunction(particle.attributes);


        particle.pBest = particle.attributes;
        particle.pBestFitness = particle.fitness;
    }
    return particle;
}

void PSOSolver::initialize_population()
{
    m_particles.resize(m_particleCount);
    for (size_t particleIndex = 0; particleIndex < m_particleCount; ++particleIndex)
    {
        m_particles[particleIndex] = generate_random_particle();
    }
}

f64 PSOSolver::average_fitness() const
{
    auto sum = azgra::collection::sum_select(m_particles.begin(), m_particles.end(), [](const PSOParticle &particle)
    { return particle.fitness; });

    f64 result = sum / static_cast<f64>(m_particleCount);
    return result;
}


f64 PSOSolver::average_p_best_fitness() const
{
    auto sum = azgra::collection::sum_select(m_particles.begin(), m_particles.end(), [](const PSOParticle &particle)
    { return particle.pBestFitness; });

    f64 result = sum / static_cast<f64>(m_particleCount);
    return result;
}

OptimizationResult PSOSolver::solve()
{
    OptimizationResult result = {};
    result.invidualsInTime.resize(m_iterationCount);
    initialize_population();

    f64 avgFitness = average_fitness();
    fprintf(stdout, "Initial average cost: %.5f\n", avgFitness);

    init_gBest(m_particles);

    for (size_t iteration = 0; iteration < m_iterationCount; ++iteration)
    {
        m_w = m_wStart - (((m_wStart - m_wEnd) * static_cast<f64>(iteration)) / static_cast<f64>(m_iterationCount));
        result.invidualsInTime[iteration] = azgra::collection::select(
                m_particles.begin(), m_particles.end(),
                [](const PSOParticle &individual)
                { return individual.attributes; });

        for (PSOParticle &particle : m_particles)
        {
            update_particle(particle);
        }

        avgFitness = average_fitness();
        double avgPBEstFitness = average_p_best_fitness();
        fprintf(stdout, "Iteration %lu/%lu average cost: %.5f avg. pBest cost: %.5f gBest cost: %.5f\n",
                iteration + 1,
                m_iterationCount,
                avgFitness,
                avgPBEstFitness,
                m_gBest.fitness);

    }

    fprintf(stdout, "Iteration %lu/%lu average cost: %.5f gBest cost: %.5f\n",
            m_iterationCount,
            m_iterationCount,
            avgFitness,
            m_gBest.fitness);
    result.result = m_gBest.fitness;
    return result;
}


void PSOSolver::update_particle(PSOParticle &particle)
{
    for (size_t dim = 0; dim < m_dimension; ++dim)
    {
        const f64 r1 = m_01rand(m_generator);
        const f64 r2 = m_01rand(m_generator);

        assert(r1 >= 0.0 && r1 <= 1.0);
        assert(r2 >= 0.0 && r1 <= 1.0);


        particle.velocity[dim] = m_w * particle.velocity[dim] +
                                 (m_cPBest * r1 * (particle.pBest[dim] - particle.attributes[dim])) +
                                 (m_cGBest * r2 * (m_gBest.attributes[dim] - particle.attributes[dim]));


        if (particle.velocity[dim] < (-1.0 * m_vMax[dim]) || particle.velocity[dim] > m_vMax[dim])
        {
            particle.velocity[dim] = m_randomVelocity[dim](m_generator);
        }
    }
    for (size_t dim = 0; dim < particle.velocity.size(); ++dim)
    {
        particle.attributes[dim] += particle.velocity[dim];

#if 1
        if (particle.attributes[dim] < m_dimensionLimits[dim].min)
        {
            particle.attributes[dim] = m_dimensionLimits[dim].min;
        }
        else if (particle.attributes[dim] > m_dimensionLimits[dim].max)
        {
            particle.attributes[dim] = m_dimensionLimits[dim].max;
        }
#else
        if ((particle.attributes[dim] < m_dimensionLimits[dim].min) || (particle.attributes[dim] > m_dimensionLimits[dim].max))
        {
            particle.attributes[dim] = m_randomAttribute[dim](m_generator);
        }
#endif
    }
    particle.fitness = m_fitnessFunction(particle.attributes);

    if (particle.fitness <= particle.pBestFitness)
    {
        particle.pBestFitness = particle.fitness;
        particle.pBest = particle.attributes;
    }
    if (particle.fitness <= m_gBest.fitness)
    {
        m_gBest.fitness = particle.fitness;
        m_gBest.attributes = particle.attributes;
        //m_gBest = particle;
    }


}

