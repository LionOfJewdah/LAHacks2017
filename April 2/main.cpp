#import <iostream>
#import "coordinateCube.h"
#import "cubieCube"
#import "search.h"
#import "util.h"

using namespace std;

int main(int argc, char* argv[])
{
	int maxDepth = 24, maxTime = 100;
	Search search;

	if (!Search::isInited()) {
		Search::init();
	}

	string r = Tools.randomCube();
	
	private void solveCube(ActionEvent evt) {
		string cubeState;

		for (int i = 0; i < 54; i++) {
			s += 'B'; // default initialization
		}

		int mask = 0;
		mask |= useSeparator ? Search.USE_SEPARATOR : 0;
		mask |= inverse ? Search.INVERSE_SOLUTION : 0;
		mask |= showLength ? Search.APPEND_LENGTH : 0;
		long t = System.nanoTime();
		String result = search.solution(cubeString, maxDepth, 100, 0, mask);;
		long n_probe = search.numberOfProbes();
		// ++++++++++++++++++++++++ Call Search.solution method from package org.kociemba.twophase ++++++++++++++++++++++++
		while (result.startsWith("Error 8") && ((System.nanoTime() - t) < maxTime * 1.0e9)) {
			result = search.next(100, 0, mask);
			n_probe += search.numberOfProbes();
		}
		t = System.nanoTime() - t;

		// +++++++++++++++++++ Replace the error messages with more meaningful ones in your language ++++++++++++++++++++++
		if (result.contains("Error")) {
			switch (result.charAt(result.length() - 1)) {
			case '1':
				result = "There are not exactly nine facelets of each color!";
				break;
			case '2':
				result = "Not all 12 edges exist exactly once!";
				break;
			case '3':
				result = "Flip error: One edge has to be flipped!";
				break;
			case '4':
				result = "Not all 8 corners exist exactly once!";
				break;
			case '5':
				result = "Twist error: One corner has to be twisted!";
				break;
			case '6':
				result = "Parity error: Two corners or two edges have to be exchanged!";
				break;
			case '7':
				result = "No solution exists for the given maximum move number!";
				break;
			case '8':
				result = "Timeout, no solution found within given maximum time!";
				break;
			}
			JOptionPane.showMessageDialog(null, result, Double.toString((t / 1000) / 1000.0) + " ms | " + n_probe + " probes", JOptionPane.INFORMATION_MESSAGE);
		} else {
			int solLen = (result.length() - (useSeparator ? 3 : 0) - (showLength ? 4 : 0)) / 3;
		}
	}
}