#include "qtslotmachine.h"
#include <QApplication>

#include "MyUtils.h"
#include "UnitTests.h"
#include "EgmReelCombination.h"
#include "Egm.h"
#include "EgmQTGui.h"

#include <string> // string
#include <iostream> // cout
using namespace std;

// BOOST
#include <boost/program_options.hpp>
namespace po = boost::program_options;

void parameterParsing(int argc, char* argv[], vector<string>& combinationFileList, int& numOfReels, int& numOfRows, int& secsPerGame) {
    stringstream ss;
    // PARAMETER PARSIMG
    ss.str(string()); // clear the ss contents.
    try {
        //// Declare the supported options.
        po::variables_map vm;
        po::options_description desc("Allowed options");
        string argFname1;
        string argFname2;
        try {
            desc.add_options()
                ("help,h", "produce help message")
                ("combination", po::value< vector<string> >()->multitoken(), "list of image files in a single reel-combination") // thank you: http://www.daniweb.com/software-development/cpp/threads/186412/list-of-arguments-with-boost-program_options
                ("reels", po::value<int>(), "number of reels to run the game with.")
                ("rows", po::value<int>(), "number of rows displayed on each reel (visual: does not affect game outcome).")
                ("secsPerGame", po::value<int>(), "number of seconds a single game will last.")
                ("unittest", "unit test the application")
                ;
            ////
            po::store(po::parse_command_line(argc, argv, desc), vm);
            po::notify(vm);
        } catch( std::exception& e) {
            ss << "Options related sxception caught: " << e.what() << ". Continuring anyway." << std::endl;
            ss << "NOTE: this won't work: BOOST will not gather other valid options." << std::endl;
            MyUtils::waitForKeystroke(ss.str().c_str(), "Exception");
        }

        ss.str(string()); // clear the ss contents.
        if (vm.count("help")) {
            ss << desc;
            MyUtils::waitForKeystroke(ss.str().c_str(), "'--help' output");
        }

        //
        // Thank you to the following web-site for showing me how-to-do a "toString" on an array:
        // http://stackoverflow.com/questions/1370323/printing-an-array-in-c
        ss.str(string()); // clear the ss contents.
        if (vm.count("combination")) {
            combinationFileList = vm["combination"].as<vector<string>>();
            ss << "'combinationFileList' set to ";
            std::copy(
                combinationFileList.begin(),
                combinationFileList.end(),
                std::ostream_iterator<std::string>(ss, " ")
            );
            ss << ".\n";
            // /**/ ss << "Include paths are: " 
            //    << vm["combination"].as< vector<string> >() << "\n";
        } else {
            combinationFileList.clear();
            ss << "'combination' not set, defaults to empty.";
        }
        MyUtils::waitForKeystroke(ss.str().c_str(), "combination");

        ss.str(string()); // clear the ss contents.
        if (vm.count("reels")) {
            numOfReels = vm["reels"].as<int>();
            ss << "'reels' set to " << numOfReels << ".\n";
        } else {
            numOfReels = 0;
            ss << "'reels' not set, defaults to " << numOfReels << ".\n";
        }
        MyUtils::waitForKeystroke(ss.str().c_str(), "Reels");

        ss.str(string()); // clear the ss contents.
        if (vm.count("rows")) {
            numOfRows = vm["rows"].as<int>();
            ss << "'rows' set to " << numOfRows << ".\n";
        } else {
            numOfRows = 0;
            ss << "'reels' not set, defaults to " << numOfRows << ".\n";
        }
        MyUtils::waitForKeystroke(ss.str().c_str(), "Rows");

        ss.str(string()); // clear the ss contents.
        if (vm.count("secsPerGame")) {
            secsPerGame = vm["secsPerGame"].as<int>();
            ss << "'secsPerGame' set to " << secsPerGame << ".\n";
        } else {
            secsPerGame = 0;
            ss << "'secsPerGame' not set, defaults to " << secsPerGame << ".\n";
        }
        MyUtils::waitForKeystroke(ss.str().c_str(), "secsPerGame");

        if (vm.count("unittest")) {
            UnitTests::testEgmQTGui(combinationFileList, numOfReels, numOfRows, secsPerGame);
            UnitTests::testReplacePixmap();
            UnitTests::testEgmQTGuiInterim();
            UnitTests::testMessageBox();
            UnitTests::testQGraphicsPixmapItem();
            UnitTests::testQGridLayout();
            UnitTests::testQHBoxLayout();
            UnitTests::testQVBoxLayout();
            UnitTests::testQVBoxLayoutReplaceWidget();
            //UnitTests::testBoostThread();
            //UnitTests::testQThreadMetaInvoke();
            UnitTests::testQThreadWorkerObject();
        }
    } catch(std::exception& e) {
        ss.str(string()); // clear the ss contents.
        ss << "Exception caught: " << e.what() << std::endl;
        MyUtils::waitForKeystroke(ss.str().c_str(), "Exception");
    }
}

int main(int argc, char *argv[])
{
    vector<string> combinationFileList; // Simple game: all reels have the same combination.
    int numOfReels, numOfRows, secsPerGame;

    parameterParsing(argc, argv, combinationFileList, numOfReels, numOfRows, secsPerGame);

    QApplication a(argc, argv); // Must execute this prior creating my GUI, or I get an error: "QWidget: Must construct a QApplication before a QPaintDevice".

    // Create the symbol combination (this game has only one, shared by all reels).
    EgmReelCombination egmReelCombination(combinationFileList);
    Egm egm(egmReelCombination, numOfReels, numOfRows, secsPerGame);
    EgmQTGui egmQTGui(NULL/*QWidget *parent*/, egm);
	egmQTGui.setWindowTitle("EgmQTGui");
	egmQTGui.show();

    //QTSlotMachine w;
    //w.show();
    return a.exec();
}
