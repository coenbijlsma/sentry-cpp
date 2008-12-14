/**
 * This file is part of Sentry.
 *
 * Sentry is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sentry is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sentry.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Sentry.h"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

void show_copyright(char*);
void show_conditions();
void show_warranty();
void show_options();

int main(int argc, char** argv){
    /* Print GPL notices */
    Sentry* sentry = 0;
    if(argc > 1){
	string arg2(argv[1]);
	if(arg2 == "-w"){
	    show_warranty(); return 0;
	}else if(arg2 == "-c"){
	    show_conditions(); return 0;
	}else if(arg2 == "--version" || arg2 == "-V"){
	    show_copyright(argv[0]); return 0;
        }else if(arg2 == "-d" || arg2 == "--debug"){
            sentry = new Sentry();
            sentry->setContext(Sentry::CONTEXT_DEBUG);
	}else{
	    cerr << "Unknown commandline option " << arg2 << endl;
            show_options(); return 0;
	}
	
    }

    if(sentry == 0){
        sentry = new Sentry();
        sentry->setContext(Sentry::CONTEXT_LIVE);
    }
    sentry->loadPlugins();
    delete sentry;
}

void show_conditions(){
    cout << "Please read the COPYING file that is supplied with Sentry," << endl;
    cout << "that saves me a lot of typing. You know, I did type the warranty" << endl;
    cout << "part, so why don't you check that out, so that wasn't all for nothing" << endl;
}

void show_warranty(){
    cout << "15. Disclaimer of Warranty." << endl;
    cout << "THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW."<< endl;
    cout << "EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS" << endl;
    cout << "AND/OR OTHER PARTIES PROVIDE THE PROGRAM “AS IS” WITHOUT WARRANTY OF" << endl;
    cout << "ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO," << endl;
    cout << "THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE." << endl;
    cout << "THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU." << endl;
    cout << "SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY" << endl;
    cout << "SERVICING, REPAIR OR CORRECTION." << endl;
    cout << endl;
    cout << "16. Limitation of Liability." << endl;
    cout << "IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL" << endl;
    cout << "ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS" << endl;
    cout << "THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY " << endl;
    cout << "GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE" << endl;
    cout << "USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF" << endl;
    cout << "DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD" << endl;
    cout << "PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS)," << endl;
    cout << "EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES." << endl;
    cout << endl;
    cout << "17. Interpretation of Sections 15 and 16." << endl;
    cout << "If the disclaimer of warranty and limitation of liability provided above cannot" << endl;
    cout << "be given local legal effect according to their terms, reviewing courts shall" << endl;
    cout << "apply local law that most closely approximates an absolute waiver of all civil" << endl;
    cout << "liability in connection with the Program, unless a warranty or assumption" << endl;
    cout << "of liability accompanies a copy of the Program in return for a fee." << endl;
}

void show_copyright(char* argv0){
    cout << "Sentry Copyright (C) 2008  Coen Bijlsma" << endl;
    cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type `" << argv0 << " -w'." << endl;
    cout << "This is free software, and you are welcome to redistribute it" << endl;
    cout << "under certain conditions; type `" << argv0 << " -c' for details." << endl;
    cout << endl;
    cout << "Written by Coen Bijlsma" << endl;
}

void show_options(){
    cout << "Sentry supports the following commandline options:" << endl;
    cout << "-w\t\tShows the warranty" << endl;
    cout << "-c\t\tShows the copyright notice" << endl;
    cout << "-V, --version\t\tShows the version of Sentry" << endl;
    cout << "-d, --debug\t\tRuns Sentry in debug mode, hence more verbose output" << endl;
}
