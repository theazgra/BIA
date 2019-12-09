#include "TLBOSolver.h"


TLBOSolver::TLBOSolver(const OptimizationProblem &problem, size_t populationSize) : Solver(problem, populationSize)
{
    m_randomStudent = std::uniform_int_distribution<size_t>(0, m_populationSize - 1);
}

OptimizationResult TLBOSolver::solve()
{
    OptimizationResult result = {};
    result.invidualsInTime.resize(m_iterationCount);
    generate_initial_students();

    f64 teacherFactor;
    set_teacher();
    for (size_t it = 0; it < m_iterationCount; ++it)
    {
        result.invidualsInTime[it] = azgra::collection::select(
                m_students.begin(), m_students.end(),
                [](const Student &student)
                { return student.attributes; });
        const auto studentsMean = get_current_students_mean();
        for (size_t currentStudentIndex = 0; currentStudentIndex < m_populationSize; ++currentStudentIndex)
        {
            Student &currentStudent = m_students[currentStudentIndex];
            teacherFactor = 1.0 + m_rand01(m_mt);

            std::vector<f64> newAttributes(m_dimensionCount);
            for (size_t dim = 0; dim < m_dimensionCount; ++dim)
            {
                newAttributes[dim] = currentStudent.attributes[dim] +
                                     (m_rand01(m_mt) * (m_teacher.attributes[dim] - (teacherFactor * studentsMean[dim])));
            }
            f64 newAttributesCost = m_fitnessFunction(newAttributes);
            if (newAttributesCost < currentStudent.fitness)
            {
                currentStudent.attributes = newAttributes;
                currentStudent.fitness = newAttributesCost;
            }

            size_t randomIndex = m_randomStudent(m_mt);
            while (randomIndex == currentStudentIndex)
            {
                randomIndex = m_randomStudent(m_mt);
            }

            const Student &randomStudent = m_students[randomIndex];
            bool moveToRandom = currentStudent.fitness > randomStudent.fitness;

            for (size_t dim = 0; dim < m_dimensionCount; ++dim)
            {
                newAttributes[dim] = currentStudent.attributes[dim] + (m_rand01(m_mt) *
                                                                       static_cast<f64>
                                                                       (moveToRandom ? (randomStudent.attributes[dim] -
                                                                                        currentStudent.attributes[dim])
                                                                                     : (currentStudent.attributes[dim] -
                                                                                        randomStudent.attributes[dim])));
            }
            newAttributesCost = m_fitnessFunction(newAttributes);
            if (newAttributesCost < currentStudent.fitness)
            {
                currentStudent.attributes = newAttributes;
                currentStudent.fitness = newAttributesCost;
            }
        }
        set_teacher();

        const f64 teacherCost = m_teacher.fitness;
        const f64 averageFitness = average_fitness();
        fprintf(stdout, "Avg: %.5f Best: %.4f\n", averageFitness, teacherCost);

    }

    return result;
}

f64 TLBOSolver::average_fitness() const
{
    const f64 sum = azgra::collection::sum(m_students.begin(), m_students.end(), [](const Student &student)
    { return student.fitness; }, 0.0f);
    const f64 avgFit = sum / static_cast<f64>(m_populationSize);
    return avgFit;
}


void TLBOSolver::generate_initial_students()
{
    auto individuals = generate_random_individuals();
    m_students.resize(m_populationSize);
    for (size_t i = 0; i < m_populationSize; ++i)
    {
        m_students[i].attributes = std::move(individuals[i].attributes);
        m_students[i].fitness = individuals[i].fitness;
    }
}

void TLBOSolver::set_teacher()
{
    m_teacher = m_students[0];
    for (size_t i = 1; i < m_populationSize; ++i)
    {
        if (m_students[i].fitness < m_teacher.fitness)
        {
            m_teacher = m_students[i];
        }
    }
}

std::vector<f64> TLBOSolver::get_current_students_mean() const
{
    std::vector<f64> mean(m_dimensionCount, 0.0);
    for (const Student &student : m_students)
    {
        for (int dim = 0; dim < m_dimensionCount; ++dim)
        {
            mean[dim] += student.attributes[dim];
        }
    }

    for (int dim = 0; dim < m_dimensionCount; ++dim)
    {
        mean[dim] /= static_cast<f64>(m_populationSize);
    }
    return mean;
}
