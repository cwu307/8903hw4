#if !defined(__ErrorDef_hdr__)
#define __ErrorDef_hdr__

enum Error_t
{
    kNoError,

    kFileOpenError,
    kFileAccessError,

    kFunctionInvalidArgsError,

    kNotInitializedError,

    kMemError,

    kUnknownError,

    kNumErrors
};
#endif // #if !defined(__ErrorDef_hdr__)



