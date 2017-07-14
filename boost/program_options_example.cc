#include <boost/program_options.hpp>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;
namespace po = boost::program_options;

int main(int ac, char* av[])
{
    int opt;
    // Declare a group of options that will be
    // allowed only on command line
    po::options_description generic("Generic options");
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
        ;

    // Declare a group of options that will be
    // allowed both on command line and in
    // config file
    po::options_description config("Configuration");
    config.add_options()
        ("optimization", po::value<int>(&opt)->default_value(10),
            "optimization level")
        ("include-path,I",
            po::value< vector<string> >()->composing(),
            "include path")
        ;

    // Hidden options, will be allowed both on command line and
    // in config file, but will not be shown to the user.
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("input-file", po::value< vector<string> >(), "input file")
        ;

    po::positional_options_description p;
    p.add("input-file", -1);

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    po::options_description config_file_options;
    config_file_options.add(config).add(hidden);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config);

    try {
        po::variables_map vm;

        // load config.ini file
        std::ifstream config_file("config.ini", std::ifstream::in);
        po::store( po::parse_config_file(config_file, config_file_options), vm);
        config_file.close();

        // command line will cover the config.ini if setting conflict
        po::store(po::command_line_parser(ac, av).options(cmdline_options).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << visible << "\n";
            return 1;
        }

        if (vm.count("include-path")) {
            cout << "Include paths are: " ;
            for (auto& str : vm["include-path"].as< vector<string> >()) {
                cout << str << ", ";
            }
            cout << "\n";
        }

        if (vm.count("input-file")) {
            cout << "Input files are: " ;
            for (auto& str : vm["input-file"].as< vector<string> >()) {
                cout << str << ", ";
            }
            cout << "\n";
        }

        cout << "Optimization level is " << opt << "\n";
    } catch (exception &ex) {
        cout << visible << "\n";
        return 1;
    }
    return 0;
}

