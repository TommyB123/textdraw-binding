#include <PawnPlus>

static enum e_TEXTDRAW_BINDING
{
    binded_callback[64],
    binded_extra_value
}

//map of textdraw bindings
static Map:TextBindings;

//map of per-player textdraw bindings
static Map:PlayerTextBindings[MAX_PLAYERS];

//callback handlers
static CallbackHandler:PlayerDisconnectHandler = INVALID_CALLBACK_HANDLER;
static CallbackHandler:PlayerClickTextDrawHandler = INVALID_CALLBACK_HANDLER;
static CallbackHandler:PlayerClickPlayerTDHandler = INVALID_CALLBACK_HANDLER;

//native handlers
static NativeHook:TextdrawDestroyHook = INVALID_NATIVE_HOOK;
static NativeHook:PlayerTextDrawDestroyHook = INVALID_NATIVE_HOOK;

forward _pp@on_init@tbinder();
public _pp@on_init@tbinder()
{
    wait_ticks(1); //this apparently fixes some random YSI incompatibility

    TextBindings = map_new();

    PlayerDisconnectHandler = pawn_register_callback("OnPlayerDisconnect", "TBinding_PlayerDisconnect");
    PlayerClickTextDrawHandler = pawn_register_callback("OnPlayerClickTextDraw", "TBinding_TextdrawClick");
    PlayerClickPlayerTDHandler = pawn_register_callback("OnPlayerClickPlayerTextDraw", "TBinding_PlayerTextdrawClick");

    TextdrawDestroyHook = pawn_add_filter("TextDrawDestroy", "TBinding_TextDrawDestroy", "i", filter_out);
    PlayerTextDrawDestroyHook = pawn_add_filter("PlayerTextDrawDestroy", "TBinding_PlayerTextDrawDestroy", "ii", filter_out);
}

forward _pp@on_exit@tbinder();
public _pp@on_exit@tbinder()
{
    pawn_unregister_callback(PlayerDisconnectHandler);
    pawn_unregister_callback(PlayerClickTextDrawHandler);
    pawn_unregister_callback(PlayerClickPlayerTDHandler);

    pawn_remove_hook(TextdrawDestroyHook);
    pawn_remove_hook(PlayerTextDrawDestroyHook);

    PlayerDisconnectHandler = INVALID_CALLBACK_HANDLER;
    PlayerClickTextDrawHandler = INVALID_CALLBACK_HANDLER;
    PlayerClickPlayerTDHandler = INVALID_CALLBACK_HANDLER;

    TextdrawDestroyHook = INVALID_NATIVE_HOOK;
    PlayerTextDrawDestroyHook = INVALID_NATIVE_HOOK;
}

forward TBinding_PlayerDisconnect(playerid, reason);
public TBinding_PlayerDisconnect(playerid, reason)
{
    if(map_valid(PlayerTextBindings[playerid]))
    {
        map_delete(PlayerTextBindings[playerid]);
    }

    PlayerTextBindings[playerid] = INVALID_MAP;
}

forward TBinding_TextdrawClick(playerid, Text:clickedid);
public TBinding_TextdrawClick(playerid, Text:clickedid)
{
    if(IsValidTextDraw(clickedid) && map_valid(TextBindings) && map_has_key(TextBindings, _:clickedid))
    {
        new binding[e_TEXTDRAW_BINDING];
        map_get_arr(TextBindings, _:clickedid, binding);
        pawn_call_public(binding[binded_callback], "iii", playerid, _:clickedid, binding[binded_extra_value]);
    }
}

forward TBinding_PlayerTextdrawClick(playerid, PlayerText:playertextid);
public TBinding_PlayerTextdrawClick(playerid, PlayerText:playertextid)
{
    if(IsValidPlayerTextDraw(playerid, playertextid) && map_valid(PlayerTextBindings[playerid]) && map_has_key(PlayerTextBindings[playerid], _:playertextid))
    {
        new binding[e_TEXTDRAW_BINDING];
        map_get_arr(PlayerTextBindings[playerid], _:playertextid, binding);

        pawn_call_public(binding[binded_callback], "iii", playerid, _:playertextid, binding[binded_extra_value]);
    }
}

forward TBinding_TextDrawDestroy(&result, &Text:textid);
public TBinding_TextDrawDestroy(&result, &Text:textid)
{
    if(IsValidTextDraw(textid) && map_has_key(TextBindings, _:textid))
    {
        map_remove(TextBindings, _:textid);
    }
}

forward TBinding_PlayerTextDrawDestroy(&result, &playerid, &PlayerText:textid);
public TBinding_PlayerTextDrawDestroy(&result, &playerid, &PlayerText:textid)
{
    if(IsValidPlayerTextDraw(playerid, textid) && map_valid(PlayerTextBindings[playerid]) && map_has_key(PlayerTextBindings[playerid], _:textid))
    {
        map_remove(PlayerTextBindings[playerid], _:textid);

        if(!map_size(PlayerTextBindings[playerid]))
        {
            map_delete(PlayerTextBindings[playerid]);
            PlayerTextBindings[playerid] = INVALID_MAP;
        }
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