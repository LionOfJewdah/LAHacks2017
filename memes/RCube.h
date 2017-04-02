/*
 *
 *----------------------------------------------------------------------------
 * Rubik Cube solver.
 * Copyright (C) Marek Karcz 2015. All rights reserved.
 *
 * License agreement between the User and the Author:
 *
 * This software and its source code is free to use and distribute
 * for non-commercial purposes only and as long as following conditions
 * are met:
 *
 * - this copyright notice in its entirety along with
 *   the license agreement and disclaimer notice are included
 *   in the distributed package,
 *
 * - the source code is included in distributed package.
 *
 * User is free to modify the code and distribute the modified code
 * as long as the modifications are properly documented and also
 * included in the distributed package.
 * Following forms of documentation of the changes to the code are
 * accepted:
 *
 * - source code comments,
 *
 * - description of the changes in the form of a readme document
 *   in any commonly used format (text, ms office, open office).
 *
 * Disclaimer:
 *
 * No warranty is offered in any form. Author can not be held
 * responsible for any damage caused by the software.
 * Use at your own risk.
 *----------------------------------------------------------------------------
 *
 */
 
#ifndef _RCUBE_H
#define _RCUBE_H

namespace Rubik
{

typedef int Face[3][3];

// facelet model
//
struct RCube {
	// state
	Face front;
	Face up;
	Face down;
	Face left;
	Face right;
	Face bottom;

	// some useful linking data from parent node
	RCube *parent;
	int parent_move;
	int cube_id;

	RCube() {
		parent = NULL;
		cube_id = (int)this;
		parent_move = 0;
	}

	RCube(const RCube &p) {
		parent = NULL;
		cube_id = (int)this;
		parent_move = 0;
		if (NULL != &p) {
			parent = (RCube *)&p;
			for (int i=0; i<3; i++)
				for (int j=0; j<3; j++) {
					front[i][j]  = p.front[i][j];
					up[i][j]     = p.up[i][j];
					down[i][j]   = p.down[i][j];
					left[i][j]   = p.left[i][j];
					right[i][j]  = p.right[i][j];
					bottom[i][j] = p.bottom[i][j];
				}
		}
	}

	~RCube() {
	}

	void NullParent() {
		parent = NULL;
		parent_move = 0;
	}
};


} // namespace Rubik

#endif // _RCUBE_H
