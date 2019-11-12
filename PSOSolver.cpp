#include "PSOSolver.h"

PSOSolver::PSOSolver(const optimalization::OptimalizationProblem &problem, const size_t particleCount, const size_t iterationCount)
{
    m_dimension = problem.dimensionCount;
    m_dimensionLimits = problem.dimensionLimits;
    m_fitnessFunction = problem.testFunction;
    m_iterationCount = iterationCount;
    m_particleCount = particleCount;

    m_generator = std::mt19937(m_rd());
    m_01rand = std::uniform_real_distribution<f64>(0.0, 1.0);

    m_vMax.resize(m_dimension);
    m_dimensionSize.resize(m_dimension);
    for (size_t dim = 0; dim < m_dimension; ++dim)
    {
        const f64 dimMin = m_dimensionLimits[dim].min;
        const f64 dimMax = m_dimensionLimits[dim].max;
        const f64 dimensionSize = dimMin < 0 ? (dimMax + abs(dimMin)) : dimMax - dimMin;
        m_dimensionSize[dim] = dimensionSize;
        m_vMax[dim] = dimensionSize / 20.0;
    }
}

void PSOSolver::set_gBest(const std::vector<PSOIndividual> &particles)
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

PSOIndividual PSOSolver::generate_random_individual()
{
    PSOIndividual particle = {};
    particle.attributes.resize(m_dimension);
    particle.velocity.resize(m_dimension);
    for (size_t dim = 0; dim < m_dimension; ++dim)
    {
        const f64 dimMin = m_dimensionLimits[dim].min;
        const f64 dimMax = m_dimensionLimits[dim].max;
        const f64 dimensionSize = dimMin < 0 ? (dimMax + abs(dimMin)) : dimMax - dimMin;

        particle.attributes[dim] = dimMin + (m_01rand(m_generator) * (dimMax - dimMin));
        particle.velocity[dim] = 0; //= (m_01rand(m_generator) * (dimensionSize / 25.0));
        particle.pBest = particle.attributes;
        particle.fitness = m_fitnessFunction(particle.attributes);
        particle.pBestFitness = particle.fitness;
    }
    return particle;
}

void PSOSolver::initialize_population()
{
    m_particles.resize(m_particleCount);
    for (size_t particleIndex = 0; particleIndex < m_particleCount; ++particleIndex)
    {
        m_particles[particleIndex] = generate_random_individual();
    }
}

f64 PSOSolver::average_fitness() const
{
    f64 result = 0.0;
    for (const auto &particle : m_particles)
    {
        result += particle.fitness;
    }
    result /= static_cast<f64>(m_particleCount);
    return result;
}

OptimizationResult PSOSolver::solve()
{
    OptimizationResult result = {};
    result.invidualsInTime.resize(m_iterationCount);
    initialize_population();
    f64 avgFitness = average_fitness();
    fprintf(stdout, "Initial average cost: %.5f\n", avgFitness);

    set_gBest(m_particles);

    for (size_t iteration = 0; iteration < m_iterationCount; ++iteration)
    {
        result.invidualsInTime[iteration] = azgra::collection::select(
                m_particles.begin(), m_particles.end(),
                [](const PSOIndividual &individual)
                { return individual.attributes; });

        for (PSOIndividual &particle : m_particles)
        {
            update_particle(particle);
        }
        avgFitness = average_fitness();
        fprintf(stdout, "Iteration %lu/%lu average cost: %.5f gBest cost: %.5f\n", iteration + 1, m_iterationCount, avgFitness,
                m_gBest.fitness);

    }
    fprintf(stdout, "Iteration %lu/%lu average cost: %.5f gBest cost: %.5f\n", m_iterationCount, m_iterationCount, avgFitness,
            m_gBest.fitness);
    result.result = m_gBest.fitness;
    return result;
}


void PSOSolver::update_particle(PSOIndividual &particle)
{
    std::vector<f64> newVelocity(particle.velocity.size());
    for (size_t dim = 0; dim < particle.velocity.size(); ++dim)
    {
        newVelocity[dim] = particle.velocity[dim] +
                           (m_c1 * m_01rand(m_generator) * (particle.pBest[dim] - particle.attributes[dim])) +
                           (m_c2 * m_01rand(m_generator) * (m_gBest.attributes[dim] - particle.attributes[dim]));

        // TODO(Moravec): If newVelocity[dim] > VMAX -> Regenerate velocity in bounds, Random in bounds.

        while (newVelocity[dim] > m_vMax[dim])
        {
            newVelocity[dim] *= 0.5;
            //fprintf(stdout,"velocity outside bounds\n");
        }


    }
    particle.velocity = std::move(newVelocity);

    for (size_t dim = 0; dim < particle.velocity.size(); ++dim)
    {
        particle.attributes[dim] = particle.attributes[dim] + particle.velocity[dim];

        // TODO(Moravec): If particle.attributes[dim] > dimensionLimits[dim].max --> regenerate value in given dimension
        // TODO(Moravec): If particle.attributes[dim] < dimensionLimits[dim].min --> regenerate value in given dimension
    }
    const f64 newFitness = m_fitnessFunction(particle.attributes);
    particle.fitness = newFitness;

    if (newFitness < particle.pBestFitness)
    {
        particle.pBestFitness = newFitness;
        particle.pBest = particle.attributes;

        if (newFitness < m_gBest.fitness)
        {
            m_gBest = particle;
        }
    }
}