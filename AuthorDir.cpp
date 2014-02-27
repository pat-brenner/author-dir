// AuthorDir.cpp : Create simplified setup authoring for the contents of a folder.
//

#include "stdafx.h"
#include <ostream>
#include <iostream>
#include <exception>
#include <filesystem>

using namespace std;
using namespace std::tr2::sys;

int __cdecl authorfile(const char *file, const char * folder, const char *startfolder, const char *sourcestring, const char *targetstring, bool readonly, bool localize)
{
    cout << "    <File Source=\"";
    cout << sourcestring;
    if (sourcestring[strlen(sourcestring) - 1] != '\\')
    {
        cout << "\\";
    }
    cout << file + strlen(startfolder);
    cout << "\"";
    cout << " Target=\"";
    cout << targetstring;
    if (targetstring[strlen(targetstring) - 1] != '\\')
    {
        cout << "\\";
    }
    if (strlen(folder) > strlen(startfolder))
    {
        cout << folder + strlen(startfolder) << "\\";
    }
    cout << "\"";
    if (localize)
    {
        cout << " Localize=\"native\"";
    }
    if (readonly)
    {
        cout << " PostProcess=\"readonly\"";
    }
    cout << "/>" << endl;
    return 0;
}

int __cdecl authordir(const char *startfolder, const char *sourcestring, const char *targetstring, bool recursive, bool readonly, bool localize)
{
    try
    {
        recursive_directory_iterator i(startfolder), end;
        for (; i != end; ++i)
        {
            if (!recursive)
            {
                i.no_push();
            }

            if (is_regular_file(i->path()))
            {
                authorfile(i->path().file_string().c_str(), i->path().branch_path().file_string().c_str(), startfolder, sourcestring, targetstring, readonly, localize);
            }
        }
    }
    catch (exception &e)
    {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}

void __cdecl usage()
{
    cout << "usage: authordir <foldername> <sourcedirstring> <targetdirstring> [recursive] [readonly] [localize]" << endl;
}

int __cdecl main(int argc, char *argv[])
{
    if (argc < 4)
    {
        usage();
        return 1;
    }

    char *startfolder = argv[1], *sourcestring = argv[2], *targetstring = argv[3];
    bool recursive = ((argc > 4 && (!_stricmp(argv[4], "recursive"))) || (argc > 5 && (!_stricmp(argv[5], "recursive"))) || (argc > 6 && (!_stricmp(argv[6], "recursive"))));
    bool readonly = ((argc > 4 && (!_stricmp(argv[4], "readonly"))) || (argc > 5 && (!_stricmp(argv[5], "readonly"))) || (argc > 6 && (!_stricmp(argv[6], "readonly"))));
    bool localize = ((argc > 4 && (!_stricmp(argv[4], "localize"))) || (argc > 5 && (!_stricmp(argv[5], "localize"))) || (argc > 6 && (!_stricmp(argv[6], "localize"))));

    cout << "<Module xmlns=\"http://schemas.microsoft.com/developer/modules/2012\">" << endl;
    cout << "  <Facet Domain=\"setup\">" << endl;

    int ret = authordir(startfolder, sourcestring, targetstring, recursive, readonly, localize);

    cout << "  </Facet>" << endl;
    cout << "</Module>" << endl;

    return ret;
}
