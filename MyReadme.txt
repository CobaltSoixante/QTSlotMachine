QTDIR and BOOSTDIR settings:
QT and BOOST should both be configured, here are examples of QTDIR and BOOSTDIR environment variables:
QTDIR=C:\Qt\5.9.1\msvc2017_64
BOOSTDIR=C:\Program Files\boost\boost_1_65_1

The above will work in DEBUG mode of this app.

COMMAND OPTIONS:
            desc.add_options()
                ("help,h", "produce help message")
                ("combination", po::value< vector<string> >()->multitoken(), "list of image files in a single reel-combination") // thank you: http://www.daniweb.com/software-development/cpp/threads/186412/list-of-arguments-with-boost-program_options
                ("reels", po::value<int>(), "number of reels to run the game with.")
                ("rows", po::value<int>(), "number of rows displayed on each reel (visual: does not affect game outcome).")
                ("secsPerGame", po::value<int>(), "number of seconds a single game will last.")
                ("unittest", "unit test the application")
                ;

Here is an example of the options given in the DEBUG properties of this program:
--reels 5  --rows 3  --secsPerGame 3  --combination .\Resources\2.jpg .\Resources\3.jpg .\Resources\4.jpg .\Resources\5.jpg .\Resources\6.jpg .\Resources\7.jpg .\Resources\8.jpg .\Resources\9.jpg .\Resources\10.jpg .\Resources\11_jack.jpg .\Resources\12_queen.jpg .\Resources\13_king.jpg .\Resources\14_ace.jpg

Running this project in DEBUG mode on your PC - by simply building and pressing F5 - should be fine.
If you run the release version - you'll need to add the %QTDIR%\bin to your PATH environment-variable (so it picks up the QT dll's).

