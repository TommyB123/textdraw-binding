#include <PawnPlus>
#include <pp-hooks>

static enum e_TEXTDRAW_BINDING
{
    binded_callback[64],
    binded_extra_value
}

//map of textdraw bindings
static Map:TextBindings;

//map of player textdraw bindings
static Map:PlayerTextBindings[MAX_PLAYERS];

hook OnGameModeInit()
{
    TextBindings = map_new();
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(map_valid(PlayerTextBindings[playerid]))
    {
        map_delete(PlayerTextBindings[playerid]);
    }

    PlayerTextBindings[playerid] = INVALID_MAP;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(IsValidTextDraw(clickedid) && map_valid(TextBindings) && map_has_key(TextBindings, _:clickedid))
    {
        new binding[e_TEXTDRAW_BINDING];
        map_get_arr(TextBindings, _:clickedid, binding);
        pawn_call_public(binding[binded_callback], "iii", playerid, _:clickedid, binding[binded_extra_value]);
    }
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if(IsValidPlayerTextDraw(playerid, playertextid) && map_valid(PlayerTextBindings[playerid]) && map_has_key(PlayerTextBindings[playerid], _:playertextid))
    {
        new binding[e_TEXTDRAW_BINDING];
        map_get_arr(PlayerTextBindings[playerid], _:playertextid, binding);

        pawn_call_public(binding[binded_callback], "iii", playerid, _:playertextid, binding[binded_extra_value]);
    }
}

stock BindTextdrawToCallback(Text:textdraw, const callback[], extravalue = 0)
{
    if(!pawn_public_exists(callback))
    {
        return false;
    }

    if(!IsValidTextDraw(textdraw))
    {
        return false;
    }

    if(!TextDrawIsSelectable(textdraw))
    {
        return false;
    }

    new binding[e_TEXTDRAW_BINDING];
    binding[binded_extra_value] = extravalue;
    format(binding[binded_callback], 64, callback);
    map_add_arr(TextBindings, _:textdraw, binding);
    return true;
}

stock BindPlayerTextdrawToCallback(playerid, PlayerText:textdraw, const callback[], extravalue = 0)
{
    if(!map_valid(PlayerTextBindings[playerid]))
    {
        PlayerTextBindings[playerid] = map_new();
    }

    if(!pawn_public_exists(callback))
    {
        return false;
    }

    if(!IsValidPlayerTextDraw(playerid, textdraw))
    {
        return false;
    }

    if(!PlayerTextDrawIsSelectable(playerid, textdraw))
    {
        return false;
    }

    new binding[e_TEXTDRAW_BINDING];
    binding[binded_extra_value] = extravalue;
    format(binding[binded_callback], 64, callback);
    map_add_arr(PlayerTextBindings[playerid], _:textdraw, binding);
    return true;
}

stock UnbindTextDraw(Text:textdraw)
{
    if(IsValidTextDraw(textdraw) && map_has_key(TextBindings, _:textdraw))
    {
        map_remove(TextBindings, _:textdraw);
    }
}

stock UnbindPlayerTextdraw(playerid, PlayerText:textdraw)
{
    if(!map_valid(PlayerTextBindings[playerid]))
    {
        return false;
    }

    if(IsValidPlayerTextDraw(playerid, textdraw) && map_has_key(PlayerTextBindings[playerid], _:textdraw))
    {
        map_remove(PlayerTextBindings[playerid], _:textdraw);
    }

    if(!map_size(PlayerTextBindings[playerid]))
    {
        map_delete(PlayerTextBindings[playerid]);
        PlayerTextBindings[playerid] = INVALID_MAP;
    }
    return true;
}