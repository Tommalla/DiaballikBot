#ifndef BOTENUMS_H
#define BOTENUMS_H

enum GTPCommand {
	GTP_PLAY = 0,
	GTP_GEN_MOVE = 1,
	GTP_UNDO_TURN = 2,
	GTP_NEW_GAME = 3,
	GTP_UNKNOWN = 666
};

#endif