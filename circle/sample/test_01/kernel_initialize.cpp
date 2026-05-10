boolean			CKernel::Initialize						(	void )
{
                boolean bOK = TRUE;

                if (bOK)
                    {
                    bOK = frameBufferInit();
                    }

                if (bOK)
                    {
                    bOK = m_Interrupt.Initialize();
                    }

                if (bOK)
                    {
                    bOK = m_Timer.Initialize();
                    }

                if (bOK)
                    {
                    CLogger::SetRawSink(CKernel::LoggerSink, this);

                    bOK = m_Logger.Initialize(0);
                    }

                return bOK;
}