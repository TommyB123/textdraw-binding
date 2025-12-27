# textdraw-binding

[![sampctl](https://img.shields.io/badge/sampctl-textdraw--binding-2f2f2f.svg?style=for-the-badge)](https://github.com/TommyB123/textdraw-binding)

This is a simple SA-MP/open.mp library that adds two functions to bind global and per-player textdraws to specific callback and pass along extra IDs to it.

Example:
```pawn
// bind a preview model box to the OnPlayerClickCharacterTextdraw callback while passing along the ID of the respective character
BindPlayerTextdrawToCallback(playerid, CharacterSelectionBoxes[playerid][x], "OnPlayerClickCharacterTextdraw", characterIDs[x]);

//create a custom callback for the textdraw click. note the characterid argument.
forward OnPlayerClickCharacterTextdraw(playerid, PlayerText:textdraw, characterid);
public OnPlayerClickCharacterTextdraw(playerid, PlayerText:textdraw, characterid)
{
	CancelSelectTextDraw(playerid);

	TextDrawHideForPlayer(playerid, CharacterSelectionMain);
	TextDrawHideForPlayer(playerid, CharacterSelectionLogout);
	DestroyCharacterSelectionTDs(playerid);

	CharacterLogin(playerid, characterid, FetchCharacterName(characterid)); //using the characterid argument we passed from BindPlayerTextdrawToCallback!
}
```

# Dependencies
This library requires the following dependencies.
* [PawnPlus](https://github.com/IS4Code/PawnPlus)

# Installation

Simply install to your project:

```bash
sampctl package install TommyB123/textdraw-binding
```

Include in your code and begin using the library:

```pawn
#include <textdraw-binding>
```

# Functions
```pawn
BindTextdrawToCallback(Text:textdraw, const callback[], extravalue = 0)
```

Binds a global textdraw to a callback. The callback in question must have 3 arguments to accommodate for the extra value being passed along.

```pawn
BindPlayerTextdrawToCallback(playerid, PlayerText:textdraw, const callback[], extravalue = 0)
```

Binds a per-player textdraw to a callback. The callback in question must have 3 arguments to accommodate for the extra value being passed along.
