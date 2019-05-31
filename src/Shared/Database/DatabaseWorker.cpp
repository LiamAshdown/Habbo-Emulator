#include "DatabaseWorker.h"
#include "Operator.h"

namespace SteerStone
{
    /// Constructor
    DatabaseWorker::DatabaseWorker()
    {
        m_WorkerThread = std::thread(&DatabaseWorker::WorkerThread, this);
    }

    /// Deconstructor
    DatabaseWorker::~DatabaseWorker()
    {
        m_Queue.ShutDown();
        m_WorkerThread.join();
    }

    /// WorkerThread
    /// Executes asynchronous queries
    void DatabaseWorker::WorkerThread()
    {
        while (true)
        {
            Operator* l_Operator = nullptr;
            m_Queue.Pop(l_Operator);

            if (l_Operator)
            {
                l_Operator->Execute();

                delete l_Operator;
            }
        }
    }
} ///< NAMESPACE STEERSTONE