// Dll1.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Dll1.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "bioman.h"
#include "model.pb.h"
#include <google/protobuf/util/json_util.h>


bioman bio;



//------------------------------------------------------------
// Визначення глобальної змінної
bool flag_error = false; 
string Data_Exper_Path = ""; 
string Results_Path = "";  
string Line_Error_Consol = ""; 
//------------






// This is an example of an exported variable
DLL1_API int nDll1 = 0;

// This is an example of an exported function.
DLL1_API int fnDll1(void)
{
    return 0;
}

// Constructor of the CDll1 class
CDll1::CDll1()
{
    return;
}


extern "C" DLL1_API const char* Main(char* Text_in)
{
    setlocale(LC_ALL, "");
    std::string str = "Result";

    //str += "\"TCore\":[36.942,36.925,36.91,36.902,36.899,36.901,36.907,36.916,36.929,36.945,36.962,36.983,37.006,37.03,37.058,37.087,37.116,37.149,37.183,37.216,37.252,37.289,37.325,37.364,37.373,37.356,37.334,37.317,37.305,37.297,37.292],";
    //str += "\"TSkin\":[34.332,34.646,34.925,35.148,35.328,35.497,35.649,35.779,35.905,36.021,36.122,36.221,36.312,36.392,36.472,36.547,36.613,36.68,36.744,36.802,36.862,36.92,36.973,36.91,35.791,35.422,35.287,35.254,35.255,35.268,35.283],";

    std::cout << str << std::endl;


    Experiment experiment;


    auto status = google::protobuf::util::JsonStringToMessage(Text_in, &experiment);
    if (!status.ok()) {
        cerr << "JSON deserialization error: " << status.message() << endl;

    }




    cout << "ID: " << experiment.id() << endl;
    cout << "Hash: " << experiment.hash() << endl;
    cout << "Title: " << experiment.title() << endl;


    for (const auto& stage : experiment.stagemodellist()) {
        cout << "\n\t" << "Stagemodellist\n" << endl;
        cout << "Stage ID: " << stage.id() << endl;
        cout << "Stage Hash: " << stage.hash() << endl;
        cout << "Stage Title: " << stage.title() << endl;

        // Перебір activity
        for (const auto& activ : stage.activity()) {
            cout << "\n\t" << "Activity\n" << endl;
            cout << "Activity ID: " << activ.id() << endl;
            cout << "Activity Hash: " << activ.hash() << endl;
            cout << "Intensity: " << activ.intensityinwatt() << " W" << endl;
            cout << "Duration: " << activ.durationinminutes() << " minutes" << endl;
            cout << "Type: " << activ.typeactivity() << endl;
        }

        // Перебір BodyItem
        for (const auto& body : stage.body()) {
            cout << "\n\t" << "Body\n" << endl;
            cout << "Body ID: " << body.id() << endl;
            cout << "Body Hash: " << body.hash() << endl;

            // Перебір head у BodyItem
            for (const auto& head : body.head()) {
                cout << "\n\t" << "Head\n" << endl;
                for (const auto& env : head.environment()) {
                    cout << "Head Environment ID: " << env.id() << endl;
                    cout << "Head Environment Hash: " << env.hash() << endl;
                    cout << "Head Environment Type: " << env.typeenv() << endl;
                    cout << "Head Environment Temperature: " << env.temperatureincelsious() << " °C" << endl;
                    cout << "Head Environment Humidity: " << env.humidityinpercent() << "%" << endl;
                    cout << "Head Environment Wind Velocity: " << env.windvelocity() << " m/s" << endl;
                }
            }
            for (const auto& footLeft : body.footleft()) {
                cout << "\n\t" << "FootLeft\n" << endl;
                for (const auto& env : footLeft.environment()) {
                    cout << "FootLeft Environment ID: " << env.id() << endl;
                    cout << "FootLeft Environment Hash: " << env.hash() << endl;
                    cout << "FootLeft Environment Type: " << env.typeenv() << endl;
                    cout << "FootLeft Environment Temperature: " << env.temperatureincelsious() << " °C" << endl;
                    cout << "FootLeft Environment Humidity: " << env.humidityinpercent() << "%" << endl;
                    cout << "FootLeft Environment Wind Velocity: " << env.windvelocity() << " m/s" << endl;
                }

                // Перебір Clothing у footLeft
                for (const auto& clothing : footLeft.clothing()) {
                    cout << "FootLeft Clothing ID: " << clothing.id() << endl;
                    cout << "FootLeft Clothing Hash: " << clothing.hash() << endl;
                    cout << "FootLeft Clothing Covering: " << clothing.covering() << endl;

                    for (const auto& fabric : clothing.fabric()) {
                        cout << "FootLeft Clothing Fabric ID: " << fabric.id() << endl;
                        cout << "FootLeft Clothing Fabric Hash: " << fabric.hash() << endl;
                        cout << "FootLeft Clothing Fabric Ref: " << fabric.ref() << endl;
                        cout << "FootLeft Clothing Fabric Rf: " << fabric.rf() << endl;
                        cout << "FootLeft Clothing Fabric IP: " << fabric.ip() << endl;
                        cout << "FootLeft Clothing Fabric Density: " << fabric.density() << endl;
                        cout << "FootLeft Clothing Fabric Thinckness: " << fabric.thinckness() << endl;
                    }
                }
            }
        }

    }


























    /*

    flag_error = 0;
    bio.flag_Recalc = 0;
    bio.flag_error_b = 0;
    bio.flag_android = -1;
    bio.flag_immersion = 0;
    bio.Text_Log_Error_b = "";


    bio.Data_FABRIC_Path = "D:\\UM_health_server\\UM_mhealth\\FABRIC_current.csv";
    bio.flag_Recalc = 1; //*** re sCODing after !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    fOpen_Settings_Ini();
    if (!flag_error) fOpen_Experiments_Json();

    if (!flag_error) fRun_New_Start();

    //fclose(File_Log);

    if (flag_error)
    {
        cout << "Error! ";
        cout << Line_Error_Consol.c_str() << endl;
        //fAdd_String_Log(" Error. "+Line_Error_Consol);
    }

    //cin.get();     //*** 2_comment after !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    return bio.Global_Results.c_str();


    //WriteToFile("O:\\debug\\log.txt", ss);
    //return ss;
    */
    return 0;
}


// Exported function HelloWorld without name decoration
extern "C" DLL1_API const char* HelloWorld(char* Text_in)
{
  

    TCHAR exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);
    // Видалення ім'я виконуваного файлу із шляху
    for (int i = lstrlen(exePath) - 1; i >= 0; i--) {
        if (exePath[i] == '\\') {
            exePath[i] = '\0';
            break;
        }
    }

    // Шлях до вашого файлу
    const wchar_t* fileName = L"\\test_air.txt";
    wchar_t inputFilePath[MAX_PATH];
    wcscpy_s(inputFilePath, exePath); // Копіюємо шлях до exe файла
    wcscat_s(inputFilePath, MAX_PATH, fileName); // Додаємо ім'я файлу

    // Конвертуємо std::wstring в std::string
    std::wstring wideInputFilePath(inputFilePath);
    std::string myFile(wideInputFilePath.begin(), wideInputFilePath.end());

    return myFile.c_str();

}

// Exported function Test without name decoration
extern "C" DLL1_API std::string Test(char* Text_in)
{
    std::string str = "Result";
    str += "\n";
    str += Text_in;
    const char* ss = str.c_str();
    return str;
}

extern "C" DLL1_API void WriteToFile(const char* data) {
    std::string filename = "D:\\UM_health_server\\UM_mhealth\\logger.txt";;
    
    // Ініціалізуємо об'єкт outputFile тут
    std::ofstream outputFile;

    // Відкриття файлу для запису
    outputFile.open(filename);

    // Перевірка, чи файл був відкритий успішно
    if (outputFile.is_open()) {
        // Запис даних у файл
        outputFile << data << std::endl;

        // Закриття файлу
        outputFile.close();

        // Вивід повідомлення про успішне завершення
        std::cout << "Data has been written to the file: " << filename << std::endl;
    }
    else {
        // Вивід повідомлення про помилку, якщо файл не вдалося відкрити
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
    }
}



// -------------------

void fOpen_Settings_Ini(void)
{

    //------------------------------------
    string Open_INI_file;
    ifstream File_ini;
    bool flag_ini = 1;

   // Open_INI_file = EXE_Path;
    Open_INI_file = "D:\\UM_health_server\\UM_mhealth\\Settings.ini";

    File_ini.open(Open_INI_file.c_str());
    if (File_ini.is_open() && !File_ini.eof())
    {
        getline(File_ini, Open_INI_file);
        if (Open_INI_file != "[Settings_UM_mhealth]") flag_ini = 0;
        else if (!File_ini.eof())
        {
            getline(File_ini, Open_INI_file);
            if (Open_INI_file != "[Path_to_Results_Folder]") flag_ini = 0;
        }

        if (flag_ini) if (!File_ini.eof())
        {
            getline(File_ini, Results_Path);
            Results_Path += "\\";

            if (GetFileAttributesA(Results_Path.c_str()) == INVALID_FILE_ATTRIBUTES)
            {
                Line_Error_Consol += "File \"Settings.ini\" contains invalid path  ";
                Line_Error_Consol += Results_Path;
                Line_Error_Consol += "   ";
                flag_error = 1;
            }

            if (!File_ini.eof()) getline(File_ini, Open_INI_file);
            if (Open_INI_file != "[END]") flag_ini = 0;
        }
        File_ini.close();
    }
    else
    {
        Line_Error_Consol += "Error opening file \"Settings.ini\"   ";
        flag_error = 1;
    }

    if (!flag_ini)
    {
        Line_Error_Consol += "File \"Settings.ini\" is not correct.   ";
        flag_error = 1;
    }
}
//---------------------------------------------------------------------------

void fOpen_Experiments_Json()
{                            

    // test_water_ANDROID    //test_air
    if (Data_Exper_Path.empty())Data_Exper_Path = "D:\\UM_health_server\\UM_mhealth\\test_air.txt"; //*** 3_comment after !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (!Data_Exper_Path.empty() && (GetFileAttributesA(Data_Exper_Path.c_str()) != INVALID_FILE_ATTRIBUTES))
    {
        //fAdd_String_Log(EXE_Path.c_str()+"\n"+Data_Exper_Path+"\n");
        fReading_Json();
    }
    else
    {
        flag_error = 1;
        Line_Error_Consol += "Error opening file:  ";
        Line_Error_Consol += Data_Exper_Path;
        Line_Error_Consol += "   ";
    }
}
//---------------------------------------------------------------------------

void fReading_Json(void)                                                       // Reading a parameter from Experiments.json
{                                                                              // Open file and create parser for json
    unsigned int i_js0;
    size_t i_found;
    string Line_js0;
    vector <string> Parameters_js;                                              // Parameters from JSON to class bioman

    string clothes;
    string imm_levels;
    ifstream fileN;
    fileN.open(Data_Exper_Path.c_str());
    if (fileN.is_open())
    {
        getline(fileN, Line_js0);
        fileN.close();
    }
    else
    {
        flag_error = 1;
        Line_Error_Consol += "Error opening file:  ";
        Line_Error_Consol += Data_Exper_Path;
        Line_Error_Consol += "   ";
        return;
    }

    Parameters_js.push_back("from_android");                                    // 0
    bio.Values_js.push_back("0");
    Parameters_js.push_back("type_activities");                                 // 1
    bio.Values_js.push_back("0");
    Parameters_js.push_back("sub_type_activities");                             // 2
    bio.Values_js.push_back("0");
    Parameters_js.push_back("duration");                                        // 3
    bio.Values_js.push_back("0");              //min
    Parameters_js.push_back("distance");                                        // 4
    bio.Values_js.push_back("0");              //km
    Parameters_js.push_back("intensity");                                       // 5
    bio.Values_js.push_back("0");              //W   (1 kcal/h = 0.86 * 1 W)
    Parameters_js.push_back("downhill");                                        // 6
    bio.Values_js.push_back("0");
    Parameters_js.push_back("wind_velocity");                                   // 7
    bio.Values_js.push_back("0");              //m/s
    Parameters_js.push_back("humidity");                                        // 8
    bio.Values_js.push_back("0");
    Parameters_js.push_back("air_temperature");                                 // 9
    bio.Values_js.push_back("0");              //°C
    Parameters_js.push_back("Clothes");                                         // 10-17   (M_json_i_clo + M_Count_Cylinder_8 - 1)
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Clothes_11");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Clothes_12");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Clothes_13");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Clothes_14");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Clothes_15");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Clothes_16");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Clothes_17");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("water_temperature");                               // 18 (M_json_i_water)
    bio.Values_js.push_back("0");              //°C
    Parameters_js.push_back("Immersion_level");                                 // 19-26   (M_json_i_water + 1 + M_Count_Cylinder_8 - 1)
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Immersion_level_20");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Immersion_level_21");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Immersion_level_22");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Immersion_level_23");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Immersion_level_24");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Immersion_level_25");
    bio.Values_js.push_back("0");
    Parameters_js.push_back("Immersion_level_26");
    bio.Values_js.push_back("0");

    for (int i = 0; i < M_json_i_clo; i++)                                           // 0-9
    {
        i_found = Line_js0.find(Parameters_js[i]);
        if (i_found != string::npos)
        {
            bio.Values_js[i].clear();
            i_js0 = i_found + Parameters_js[i].size();
            for (int j = 2; (Line_js0[i_js0 + j] != ',') && (Line_js0[i_js0 + j] != '}'); j++)
            {
                bio.Values_js[i].push_back(Line_js0[i_js0 + j]);
            }
        }
        else
        {
            i_found = 0;
        }
    }

    i_found = Line_js0.find(Parameters_js[M_json_i_clo]);                         // Clothes
    if (i_found != string::npos)
    {
        i_js0 = i_found + Parameters_js[M_json_i_clo].size();
        for (int j = 4; Line_js0[i_js0 + j] != ']'; j++)
        {
            clothes.push_back(Line_js0[i_js0 + j]);
        }
        clothes.pop_back();

        clothes.push_back(']');

        bio.Values_js[M_json_i_clo].clear();
        for (int i3 = M_json_i_clo; i3 < (M_json_i_clo + M_Count_Cylinder_8 - 1); )          // 10-17
        {
            for (unsigned int i4 = 0; clothes[i4] != ']'; i4++)
            {
                if (clothes[i4] == '\"')
                {
                    i4 = i4 + 2;
                    i3 = i3 + 1;
                    bio.Values_js[i3].clear();
                    continue;
                }
                else
                {
                    bio.Values_js[i3].push_back(clothes[i4]);
                }
            }
        }
    }

    i_found = Line_js0.find(Parameters_js[M_json_i_water]);                       // 18
    if (i_found != string::npos)
    {
        bio.Values_js[M_json_i_water].clear();
        i_js0 = i_found + Parameters_js[M_json_i_water].size();
        for (int j = 2; (Line_js0[i_js0 + j] != ',') && (Line_js0[i_js0 + j] != '}'); j++)
        {
            bio.Values_js[M_json_i_water].push_back(Line_js0[i_js0 + j]);           // water_temperature
        }
        bio.flag_immersion = 1;


        i_found = Line_js0.find(Parameters_js[M_json_i_water + 1]);                  // Immersion_level
        if (i_found != string::npos)
        {
            i_js0 = i_found + Parameters_js[M_json_i_water + 1].size();
            for (int j = 4; Line_js0[i_js0 + j] != ']'; j++)
            {
                imm_levels.push_back(Line_js0[i_js0 + j]);
            }
            imm_levels.pop_back();

            imm_levels.push_back(']');

            bio.Values_js[M_json_i_water + 1].clear();
            for (int i3 = M_json_i_water + 1; i3 < (M_json_i_water + 1 + M_Count_Cylinder_8 - 1); )  // 19-26
            {
                for (unsigned int i4 = 0; imm_levels[i4] != ']'; i4++)
                {
                    if (imm_levels[i4] == '\"')
                    {
                        i4 = i4 + 2;
                        i3 = i3 + 1;
                        bio.Values_js[i3].clear();
                        continue;
                    }
                    else
                    {
                        bio.Values_js[i3].push_back(imm_levels[i4]);
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void fRun_New_Start(void)
{
    int i;
    string Line_path;
    string Line_fnr;

    bio.Initial_New_Exp_0();                                                    // Initial New Exp

    if (bio.flag_Recalc)                                                         // Recalculate the steady state
    {
        bio.flag_Recalc = 0;
        bio.SteadyState();
        if (!bio.flag_Recalc) Line_Error_Consol += "Error. Recalculation of steady state parameters did not take place.   ";
    }

    i = 0;
    if (!flag_error) i = bio.Input_Exp_Param();
    if (i > 0)
    {
        flag_error = 1;
        if (i == 5) Line_Error_Consol += "from_android ???   ";
        if (i == 4) Line_Error_Consol += "Select environmental conditions.   ";
        if (i == 3) Line_Error_Consol += "Select type of your physical activity.   ";
        if (i == 2) Line_Error_Consol += "Select your physical activity.   ";
        if (i == 1) Line_Error_Consol += "Select duration of the physical activity.   ";
    }

    if (!flag_error) bio.Start_New_Exp_0();
    if (bio.flag_error_b)
    {
        flag_error = 1;
        Line_Error_Consol += bio.Text_Log_Error_b.c_str();
    }
    else
    {
        Line_path = "D:\\UM_health_server\\UM_mhealth\\\Results";                                                  //*** change after !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (!flag_error)
        {
            Line_fnr = Line_path + "Experimental_conditions.json";
            bio.fSaving_Exp_Cond_Json(Line_fnr);
            fError_Saving_File(Line_fnr);
        }
        if (!flag_error)
        {
            Line_fnr = Line_path + "Results_full.csv";
            bio.Save_Table_Results(Line_fnr);
            fError_Saving_File(Line_fnr);
        }
        if (!flag_error)
        {
            Line_fnr = Line_path + "Results_short.json";
            if (bio.flag_android == 1)
            {
                if (bio.flag_immersion == 0)
                    bio.Android_JSON_Results(Line_fnr.c_str());
                else
                    bio.Android_JSON_Results_WATER(Line_fnr.c_str());
            }
            else bio.Web_JSON_Results(Line_fnr);
            fError_Saving_File(Line_fnr);
        }
    }
}
//---------------------------------------------------------------------------

void fError_Saving_File(string Line_esf)
{
    if (GetFileAttributesA(Line_esf.c_str()) == INVALID_FILE_ATTRIBUTES)
    {
        flag_error = 1;
        Line_Error_Consol += "Error saving file  ";
        Line_Error_Consol += Line_esf;
        Line_Error_Consol += "   ";
    }
}
//---------------------------------------------------------------------------
