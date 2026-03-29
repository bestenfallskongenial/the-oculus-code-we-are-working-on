// ---- external / global dependencies (stubs) ----
extern FILEHANDLE     g_hFile;

public:

    // ---- public members (globals by design) ----
    CFATFileSystem*        m_pFileSystem;
    DeviceNameServiceStub  m_DeviceNameService;
    USBHCIStub             m_USBHCI;
    WatchdogStub           m_Watchdog;

    volatile bool          m_bStorageAttached;

    // ---- filesystem ----
    bool        Mount              (const char* p_deviceName);
    bool        UnMount            ();

    bool        openFile           (const char* p_fileName);
    bool        closeFile          ();

    unsigned    loadToBuffer       (char* p_buffer, unsigned p_bufferSize);

    bool        saveFromBufferOld  (const char* p_fileName,
                                   const char* p_buffer,
                                   unsigned    p_bufferSize);

    bool        saveFromBuffer     (const char* p_deviceName,
                                   const char* p_fileName,
                                   const char* p_buffer,
                                   unsigned    p_bufferSize);

    void        bulkLoad           (char*       p_fileNameArray[],
                                   unsigned    p_loadedBytes[],
                                   char**      p_bufferArray,
                                   int         p_maxFiles,
                                   int&        p_validFiles,
                                   int&        p_prevFiles,
                                   unsigned    p_fileSize);

    bool        IsValidFile        (const char* pFileName,
                                   const char* extension);

    bool        scanRoot           (char**      p_fileNameArray,
                                   const char* p_fileExtension[],
                                   int         p_extentionCount,
                                   int&        p_scannedFiles,
                                   unsigned    p_maxFiles);

    // ---- usb ----
    bool        updateUSB          (const char* p_deviceName);
    static void removeUSB          (CDevice* f_partitionName, void* p_pContext);

    // ---- memory ----
    char**      alllocateBufferMEM (size_t count, size_t bufferSize);

    char**      alllocateBufferDMA (size_t count,
                                   size_t bufferSize,
                                   char** blockBaseOut,
                                   char** rawBlockOut,
                                   size_t* alignedSizeOut);

    void        clearBufferMEM     (char** buffers, size_t count);
    void        clearBufferDMA     (char** buffers, char* rawBlock);
