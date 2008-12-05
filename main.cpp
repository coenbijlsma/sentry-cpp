#include "Sentry.h"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

void show_conditions();
void show_warranty();

int main(int argc, char** argv){
    /* Print GPL notices */
    if(argc == 1){
	cout << "Sentry Copyright (C) 2008  Coen Bijlsma" << endl;
	cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type `" << argv[0] << " -w'." << endl;
	cout << "This is free software, and you are welcome to redistribute it" << endl;
	cout << "under certain conditions; type `" << argv[0] << " -c' for details." << endl;
    }else{
	string arg2(argv[1]);
	if(arg2 == "-w"){
	    show_warranty();
	}else if(arg2 == "-c"){
	    show_conditions();
	}else{
	    cerr << "Unknown commandline option " << arg2 << endl;
	}
	/* And exit */
	return 0;
    }
    
    Sentry* sentry = new Sentry();
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

