
#ifdef DLL1_EXPORTS
#define DLL1_API __declspec(dllexport)
#else
#define DLL1_API __declspec(dllimport)
#endif


#include <string>
using namespace std;

//string EXE_Path;                                                            // Path to this program
extern bool flag_error;                                                            // false (= 0) - not error. true (= 1) - ERROR!!!
//FILE *File_Log;
extern string Data_Exper_Path;                                                     // Path to experimental data from mobile device
extern string Results_Path;                                                        // Path to results folder
extern string Line_Error_Consol;



//void fOpen_Reports_Log(void);                                             // Open Reports.log
//void fAdd_String_Log(string);                                             // Add string to Reports.log
void fOpen_Experiments_Json(void);                                          // Open Experiments.json
void fReading_Json(void);                                                   // Reading a parameter from Experiments.json
void fOpen_Settings_Ini(void);                                              // Open Settings.ini
void fRun_New_Start(void);                                                  // Run / New / Start / Begin ...
void fError_Saving_File(string);                                            // There was a problem saving the file?


// Оголошення функції HelloWorld без декорації
extern "C" DLL1_API const char* HelloWorld(char* Text_in);

extern "C" DLL1_API const char* Main(char* Text_in);

extern "C" DLL1_API void WriteToFile(const char* data);

// Оголошення функції Test без декорації
extern "C" DLL1_API std::string Test(char* Text_in);

extern "C" {
    class CDll1 {
    public:
        CDll1(void);
    };
}

extern DLL1_API int nDll1;

DLL1_API int fnDll1(void);
