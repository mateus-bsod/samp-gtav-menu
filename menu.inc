//
//  dP""b8 88""Yb 888888 8888b.  88 888888  dP"Yb  .dP"Y8 
// dP   `" 88__dP 88__    8I  Yb 88   88   dP   Yb `Ybo." 
// Yb      88"Yb  88""    8I  dY 88   88   Yb   dP o.`Y8b 
//  YboodP 88  Yb 888888 8888Y"  88   88    YbodP  8bodP' 
//

//Liv Javier
//tommy_stardust


//
#if defined _menu_included
    #endinput
#endif

#define _menu_included
//

//
// 88 88b 88  dP""b8 88     88   88 8888b.  888888 .dP"Y8 
// 88 88Yb88 dP   `" 88     88   88  8I  Yb 88__   `Ybo." 
// 88 88 Y88 Yb      88  .o Y8   8P  8I  dY 88""   o.`Y8b 
// 88 88  Y8  YboodP 88ood8 `YbodP' 8888Y"  888888 8bodP' 
//

// - IsPlayerAndroid
#if !defined Menu_IsPlayerAndroid
    #error "[FMenu]: <Menu_IsPlayerAndroid> not found."
#endif

// - textdraw-streamer.inc
#tryinclude <textdraw-streamer.inc>
#if !defined _INC_textdraw_streamer
	#error "[FMenu]: <github.com/nexquery/samp-textdraw-streamer> not found."
#else
#endif

// - y_hooks
#tryinclude <YSI_Coding\y_hooks>
#if !defined _inc_y_hooks
	#error "[FMenu]: <YSI_Coding\y_hooks> not found."
#endif

#include <a_samp>

//
// 8b    d8    db     dP""b8 88""Yb  dP"Yb  .dP"Y8 
// 88b  d88   dPYb   dP   `" 88__dP dP   Yb `Ybo." 
// 88YbdP88  dP__Yb  Yb      88"Yb  Yb   dP o.`Y8b 
// 88 YY 88 dP""""Yb  YboodP 88  Yb  YbodP  8bodP' 
//

#define Menu:%0(%1,%2,%3) \
	forward Menu_%0(%1,%2,%3); public Menu_%0(%1,%2,%3)
// Exemplo de Uso: Menu:xxx(playerid, response, listitem)

#define KeyMenu:%0(%1,%2) \
	forward KeyMenu_%0(%1,%2); public KeyMenu_%0(%1,%2)
// Exemplo de Uso: KeyMenu:xxx(playerid, index)

#define Menu_Show(%0,%1, \
    MShow(%0, #%1,
// Exemplo de Uso: Menu_Show(playerid, xxx, "24/7", "Interacao com NPC");

//
// Yb    dP    db    88""Yb 88    db    Yb    dP 888888 88 .dP"Y8 
//  Yb  dP    dPYb   88__dP 88   dPYb    Yb  dP  88__   88 `Ybo." 
//   YbdP    dP__Yb  88"Yb  88  dP__Yb    YbdP   88""   88 o.`Y8b 
//    YP    dP""""Yb 88  Yb 88 dP""""Yb    YP    888888 88 8bodP' 
//

const 
    MENU_MAX_ITEMS = 500,
    MENU_ITEM_COLOR = 168430310,
    MENU_MAIN_COLOR = -1523963137,
    MENU_MAX_ITEM_PER_PAGE = 12; // 

static
    bool:ApareceuMenu[MAX_PLAYERS],
    ResponseMenu[MAX_PLAYERS],
    count_tick[MAX_PLAYERS],
    player_menu_callback[MAX_PLAYERS][32],
    player_menu_callback_key[MAX_PLAYERS][32],
    Iterator:player_items[MAX_PLAYERS]<MENU_MAX_ITEMS>,
    player_item_list[MAX_PLAYERS][MENU_MAX_ITEMS][64],
    player_item_list2[MAX_PLAYERS][MENU_MAX_ITEMS][64],
    player_item_color[MAX_PLAYERS] = {MENU_MAIN_COLOR, ...},
    PlayerText:_@m_ptd[MAX_PLAYERS][5] = {PlayerText:-1, ...},
    PlayerText:_@m_box[MAX_PLAYERS][MENU_MAX_ITEM_PER_PAGE] = {PlayerText:-1, ...},
    PlayerText:_@m_item[MAX_PLAYERS][MENU_MAX_ITEM_PER_PAGE] = {PlayerText:-1, ...},
    PlayerText:_@m_item2[MAX_PLAYERS][MENU_MAX_ITEM_PER_PAGE] = {PlayerText:-1, ...};
    //PlayerText:_@m_sub_item[MAX_PLAYERS][MENU_MAX_ITEM_PER_PAGE] = {PlayerText:-1, ...}; ko lam nua

new PlayerText: SelecAndroid[MAX_PLAYERS][4];

//
// 888888 88   88 88b 88  dP""b8  dP"Yb  888888 .dP"Y8 
// 88__   88   88 88Yb88 dP   `" dP   Yb 88__   `Ybo." 
// 88""   Y8   8P 88 Y88 Yb      Yb   dP 88""   o.`Y8b 
// 88     `YbodP' 88  Y8  YboodP  YbodP  888888 8bodP' 
//

// INFO: Adiciona um item no menu.
stock Menu_Add(playerid, const item[64], const item2[64] = "_")
{
    if(ApareceuMenu[playerid])
    {
        Menu_Hide(playerid);
    }

    new 
        index = Iter_Free(player_items[playerid]);

    if(-1 < index < MENU_MAX_ITEMS)
    {
        strcat((player_item_list[playerid][index][0] = EOS, player_item_list[playerid][index]), item);
        strcat((player_item_list2[playerid][index][0] = EOS, player_item_list2[playerid][index]), item2);
        Iter_Add(player_items[playerid], index);
        return 1;
    }
    return 0;
}

// INFO: Função Bruta do Menu_Show
stock MShow(playerid, const function[], const tittle[] = "Menu", const header[] = "Interaction")
{
    if(strlen(function) < 1) 
        return 0;
    
    if(Menu_IsPlayerAndroid(playerid)) ShowMiniMenu(playerid);

    PlayerTextDrawSetString(playerid, _@m_ptd[playerid][1], tittle);
    PlayerTextDrawSetString(playerid, _@m_ptd[playerid][3], header);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][0], player_item_color[playerid]);
    for(new i = 0; i < 5; i ++) 
    {
        PlayerTextDrawShow(playerid, _@m_ptd[playerid][i]);
    }
    Menu_ShowPage(playerid, 0);
    Menu_SelectIndex(playerid, 0);

    format(player_menu_callback[playerid], 32, "Menu_%s", function);
    format(player_menu_callback_key[playerid], 32, "KeyMenu_%s", function);
	TogglePlayerControllable(playerid, 0);

    ApareceuMenu[playerid] = true;
    return 1;
}

// INFO: Função responsavel por Tirar o Menu da Tela
stock Menu_Hide(playerid) 
{

	TogglePlayerControllable(playerid, 1);
    HideMiniMenu(playerid);
    for(new i = 0; i < 5; i ++) 
    {
        if(IsDynamicPlayerTextDrawVisible(playerid, _@m_ptd[playerid][i]))
            PlayerTextDrawHide(playerid, _@m_ptd[playerid][i]);
    }
    for(new count = 0; count < MENU_MAX_ITEM_PER_PAGE; count ++) 
    {
        if(_@m_item[playerid][count] != PlayerText:INVALID_TEXT_DRAW) 
        {
            if(IsDynamicPlayerTextDrawVisible(playerid, _@m_item[playerid][count]))
            {
                PlayerTextDrawDestroy(playerid, _@m_item[playerid][count]);
                _@m_item[playerid][count] = PlayerText:INVALID_TEXT_DRAW;
            }
        }               
        if(_@m_item2[playerid][count] != PlayerText:INVALID_TEXT_DRAW) 
        {
            if(IsDynamicPlayerTextDrawVisible(playerid, _@m_item2[playerid][count]))
            {
                PlayerTextDrawDestroy(playerid, _@m_item2[playerid][count]);
                _@m_item2[playerid][count] = PlayerText:INVALID_TEXT_DRAW;
            }
        }             
        if(_@m_box[playerid][count] != PlayerText:INVALID_TEXT_DRAW) 
        {
            if(IsDynamicPlayerTextDrawVisible(playerid, _@m_box[playerid][count]))
            {
                PlayerTextDrawDestroy(playerid, _@m_box[playerid][count]);
                _@m_box[playerid][count] = PlayerText:INVALID_TEXT_DRAW;
            }
        }               
    }
    Iter_Clear(player_items[playerid]);
    SetPVarInt(playerid, "Menu.SelectIndex", -1);

    ApareceuMenu[playerid] = false;
    return 1;
}

// INFO: Função responsavel por setar a cor principal do Menu
stock Menu_SetColor(playerid, color)
{
    player_item_color[playerid] = color;
    return 1;
}

// INFO: Função responsavel por selecionar um index
static Menu_SelectIndex(playerid, index, cur_index = -1, bool:show = true) 
{
    if(cur_index != -1) 
    {
        if(_@m_box[playerid][cur_index] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawColor(playerid, _@m_box[playerid][cur_index], MENU_ITEM_COLOR);
            if(show) PlayerTextDrawShow(playerid, _@m_box[playerid][cur_index]);
        }   
    }
    if(_@m_box[playerid][index] != PlayerText:INVALID_TEXT_DRAW) 
    {
        SetPVarInt(playerid, "Menu.SelectIndex", index);
        PlayerTextDrawColor(playerid, _@m_box[playerid][index], player_item_color[playerid]);
        if(show) PlayerTextDrawShow(playerid, _@m_box[playerid][index]);
    }

    new 
        str[22];

    count_tick[playerid] = GetTickCount() + 150;
    format(str, sizeof(str), "%d/%d", Menu_CurrentItem(playerid) + 1, Iter_Count(player_items[playerid]));
    PlayerTextDrawSetString(playerid, _@m_ptd[playerid][4], str);
    PlayerTextDrawShow(playerid, _@m_ptd[playerid][4]);     
    return 1;   
}

// INFO: Função responsavel por aparecer uma pagina em especifico
static Menu_ShowPage(playerid, page = 0) 
{
    for(new j = 0, i = page * MENU_MAX_ITEM_PER_PAGE; i < MENU_MAX_ITEM_PER_PAGE * (page + 1); i ++, j ++) 
    {
        if(Iter_Contains(player_items[playerid], i)) 
        {
            Menu_Create_TD(playerid, j, player_item_list[playerid][i], player_item_list2[playerid][i]);
        }
        else 
        {
            if(_@m_item[playerid][j] != PlayerText:-1) 
            {
                if(IsDynamicPlayerTextDrawVisible(playerid, _@m_item[playerid][j]))
                {
                    PlayerTextDrawDestroy(playerid, _@m_item[playerid][j]);
                    _@m_item[playerid][j] = PlayerText:-1;
                }
            }           
            if(_@m_item2[playerid][j] != PlayerText:-1) 
            {
                if(IsDynamicPlayerTextDrawVisible(playerid, _@m_item2[playerid][j]))
                {
                    PlayerTextDrawDestroy(playerid, _@m_item2[playerid][j]);
                    _@m_item2[playerid][j] = PlayerText:-1;
                }
            }      
            if(_@m_box[playerid][j] != PlayerText:-1) 
            {
                if(IsDynamicPlayerTextDrawVisible(playerid, _@m_box[playerid][j]))
                {
                    PlayerTextDrawDestroy(playerid, _@m_box[playerid][j]);
                    _@m_box[playerid][j] = PlayerText:-1;
                }
            }   
        }
    }
    SetPVarInt(playerid, "Menu.curPage", page);
    return 1;
}

// INFO: Função responsavel para pegar atual item no qual o player esta selecionando
static Menu_CurrentItem(playerid) 
{
    return MENU_MAX_ITEM_PER_PAGE * GetPVarInt(playerid, "Menu.curPage") + GetPVarInt(playerid, "Menu.SelectIndex");
}

// INFO: Função responsavel por pegar o maximo de itens por pagina
stock Menu_GetMaxItemPerPage(playerid, cur_page) 
{
    new 
        count = 0;
    
    for(new i = MENU_MAX_ITEM_PER_PAGE * cur_page; i < MENU_MAX_ITEM_PER_PAGE * (cur_page + 1); i ++) 
    {
        if(Iter_Contains(player_items[playerid], i))
        {
            count++;
        }
    } 
    return count;
}

// INFO: Função responsavel por criar os itens
static Menu_Create_TD(playerid, index, const item[64], const item2[64]) 
{
    if(_@m_item[playerid][index] != PlayerText:-1) 
    {
        if(IsDynamicPlayerTextDrawVisible(playerid, _@m_item[playerid][index]))
            PlayerTextDrawDestroy(playerid, _@m_item[playerid][index]);
    }
    if(_@m_item2[playerid][index] != PlayerText:-1) 
    {
        if(IsDynamicPlayerTextDrawVisible(playerid, _@m_item2[playerid][index]))
            PlayerTextDrawDestroy(playerid, _@m_item2[playerid][index]);
    }
    if(_@m_box[playerid][index] != PlayerText:-1) 
    {
        if(IsDynamicPlayerTextDrawVisible(playerid, _@m_box[playerid][index]))
            PlayerTextDrawDestroy(playerid, _@m_box[playerid][index]);
    }

    new 
        Float:Y = 204.548095 + (index * 11.300745);

    _@m_box[playerid][index] = CreatePlayerTextDraw(playerid, 498.966522, Y - 1.70 + index * 0.4, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, _@m_box[playerid][index], 110.030433, 11.699993);
    PlayerTextDrawAlignment(playerid, _@m_box[playerid][index], 1);
    PlayerTextDrawColor(playerid, _@m_box[playerid][index], MENU_ITEM_COLOR);
    PlayerTextDrawSetShadow(playerid, _@m_box[playerid][index], 0);
    PlayerTextDrawFont(playerid, _@m_box[playerid][index], 4);
    PlayerTextDrawShow(playerid, _@m_box[playerid][index]);

    /*
    
    _@m_ptd[playerid][4] = CreatePlayerTextDraw(playerid, 607.598144, 193.359298, #); // Page
    PlayerTextDrawLetterSize(playerid, _@m_ptd[playerid][4], 0.123999, 0.766222);
    PlayerTextDrawSetProportional(playerid, _@m_ptd[playerid][4], 1);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][4], 0);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][4], 3);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][4], -1);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][4], 2);    
    
     */

    _@m_item2[playerid][index] = CreatePlayerTextDraw(playerid, 500.366638 + 107.231506, Y + 0.0 + index * 0.4, item2);
    PlayerTextDrawLetterSize(playerid, _@m_item2[playerid][index], 0.184666, 0.886074);
    PlayerTextDrawSetProportional(playerid, _@m_item2[playerid][index], 1);
    PlayerTextDrawAlignment(playerid, _@m_item2[playerid][index], 3);
    PlayerTextDrawSetShadow(playerid, _@m_item2[playerid][index], 0);
    PlayerTextDrawColor(playerid, _@m_item2[playerid][index], -1);
    PlayerTextDrawFont(playerid, _@m_item2[playerid][index], 1);
    PlayerTextDrawShow(playerid, _@m_item2[playerid][index]);

    _@m_item[playerid][index] = CreatePlayerTextDraw(playerid, 500.366638 + 0.0, Y + 0.0 + index * 0.4, item);
    PlayerTextDrawLetterSize(playerid, _@m_item[playerid][index], 0.184666, 0.886074);
    PlayerTextDrawSetProportional(playerid, _@m_item[playerid][index], 1);
    PlayerTextDrawAlignment(playerid, _@m_item[playerid][index], 1);
    PlayerTextDrawSetShadow(playerid, _@m_item[playerid][index], 0);
    PlayerTextDrawColor(playerid, _@m_item[playerid][index], -1);
    PlayerTextDrawFont(playerid, _@m_item[playerid][index], 1);
    PlayerTextDrawShow(playerid, _@m_item[playerid][index]);
    return 1;
}

// INFO: Função responsavel por verificar se está na tela do Jogador
stock Menu_Showing(playerid) 
{
    //return GetPVarInt(playerid, "Menu.SelectIndex") == -1 ? 0 : 1;
    return ApareceuMenu[playerid];
}

// INFO: Função responsavel por pegar o maximo de paginas
stock Menu_GetMaxPage(playerid) 
{
    new 
        items = Iter_Count(player_items[playerid]);

    return floatround((items % MENU_MAX_ITEM_PER_PAGE == 0) ? items / MENU_MAX_ITEM_PER_PAGE : items / MENU_MAX_ITEM_PER_PAGE + 1);
}

// INFO: gancho na OnPlayerConnect para criar as textdraw para android(s).
hook OnPlayerConnect(playerid)
{

    SelecAndroid[playerid][0] = CreatePlayerTextDraw(playerid, 434.000, 219.000, "CIMA");
    PlayerTextDrawLetterSize(playerid, SelecAndroid[playerid][0], 0.268, 1.398);
    PlayerTextDrawTextSize(playerid, SelecAndroid[playerid][0], 10.000, 85.000);
    PlayerTextDrawAlignment(playerid, SelecAndroid[playerid][0], 2);
    PlayerTextDrawColor(playerid, SelecAndroid[playerid][0], -1);
    PlayerTextDrawUseBox(playerid, SelecAndroid[playerid][0], 1);
    PlayerTextDrawBoxColor(playerid, SelecAndroid[playerid][0], 150);
    PlayerTextDrawSetShadow(playerid, SelecAndroid[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, SelecAndroid[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, SelecAndroid[playerid][0], 150);
    PlayerTextDrawFont(playerid, SelecAndroid[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, SelecAndroid[playerid][0], 1);
    PlayerTextDrawSetSelectable(playerid, SelecAndroid[playerid][0], 1);

    SelecAndroid[playerid][1] = CreatePlayerTextDraw(playerid, 434.000, 239.000, "BAIXO");
    PlayerTextDrawLetterSize(playerid, SelecAndroid[playerid][1], 0.268, 1.398);
    PlayerTextDrawTextSize(playerid, SelecAndroid[playerid][1], 10.000, 85.000);
    PlayerTextDrawAlignment(playerid, SelecAndroid[playerid][1], 2);
    PlayerTextDrawColor(playerid, SelecAndroid[playerid][1], -1);
    PlayerTextDrawUseBox(playerid, SelecAndroid[playerid][1], 1);
    PlayerTextDrawBoxColor(playerid, SelecAndroid[playerid][1], 150);
    PlayerTextDrawSetShadow(playerid, SelecAndroid[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, SelecAndroid[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, SelecAndroid[playerid][1], 150);
    PlayerTextDrawFont(playerid, SelecAndroid[playerid][1], 1);
    PlayerTextDrawSetProportional(playerid, SelecAndroid[playerid][1], 1);
    PlayerTextDrawSetSelectable(playerid, SelecAndroid[playerid][1], 1);

    SelecAndroid[playerid][2] = CreatePlayerTextDraw(playerid, 434.000, 172.000, "SELECIONAR");
    PlayerTextDrawLetterSize(playerid, SelecAndroid[playerid][2], 0.268, 1.398);
    PlayerTextDrawTextSize(playerid, SelecAndroid[playerid][2], 10.000, 85.000);
    PlayerTextDrawAlignment(playerid, SelecAndroid[playerid][2], 2);
    PlayerTextDrawColor(playerid, SelecAndroid[playerid][2], -1);
    PlayerTextDrawUseBox(playerid, SelecAndroid[playerid][2], 1);
    PlayerTextDrawBoxColor(playerid, SelecAndroid[playerid][2], 150);
    PlayerTextDrawSetShadow(playerid, SelecAndroid[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, SelecAndroid[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, SelecAndroid[playerid][2], 150);
    PlayerTextDrawFont(playerid, SelecAndroid[playerid][2], 1);
    PlayerTextDrawSetProportional(playerid, SelecAndroid[playerid][2], 1);
    PlayerTextDrawSetSelectable(playerid, SelecAndroid[playerid][2], 1);

    SelecAndroid[playerid][3] = CreatePlayerTextDraw(playerid, 434.000, 192.000, "SAIR/VOLTAR");
    PlayerTextDrawLetterSize(playerid, SelecAndroid[playerid][3], 0.268, 1.398);
    PlayerTextDrawTextSize(playerid, SelecAndroid[playerid][3], 10.000, 85.000);
    PlayerTextDrawAlignment(playerid, SelecAndroid[playerid][3], 2);
    PlayerTextDrawColor(playerid, SelecAndroid[playerid][3], -1);
    PlayerTextDrawUseBox(playerid, SelecAndroid[playerid][3], 1);
    PlayerTextDrawBoxColor(playerid, SelecAndroid[playerid][3], 150);
    PlayerTextDrawSetShadow(playerid, SelecAndroid[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, SelecAndroid[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, SelecAndroid[playerid][3], 150);
    PlayerTextDrawFont(playerid, SelecAndroid[playerid][3], 1);
    PlayerTextDrawSetProportional(playerid, SelecAndroid[playerid][3], 1);
    PlayerTextDrawSetSelectable(playerid, SelecAndroid[playerid][3], 1);

    _@m_ptd[playerid][0] = CreatePlayerTextDraw(playerid, 498.966491, 140.937042, "LD_SPAC:white"); // Main BG
    PlayerTextDrawTextSize(playerid, _@m_ptd[playerid][0], 110.000000, 51.000000);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][0], player_item_color[playerid]);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][0], 0);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][0], 4);

    _@m_ptd[playerid][1] = CreatePlayerTextDraw(playerid, 554.366882, 159.129791, "_"); // Main Tittle
    PlayerTextDrawLetterSize(playerid, _@m_ptd[playerid][1], 0.400000, 1.600000);
    PlayerTextDrawSetProportional(playerid, _@m_ptd[playerid][1], 1);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][1], 2);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][1], 0);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][1], -1);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][1], 1);

    _@m_ptd[playerid][2] = CreatePlayerTextDraw(playerid, 498.966491, 191.840148, "LD_SPAC:white"); // Header BG
    PlayerTextDrawTextSize(playerid, _@m_ptd[playerid][2], 110.000000, 11.000000);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][2], 16843208);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][2], 1);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][2], 0);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][2], 4);

    _@m_ptd[playerid][3] = CreatePlayerTextDraw(playerid, 501.000061, 193.359375, "Interaction"); // Header 
    PlayerTextDrawLetterSize(playerid, _@m_ptd[playerid][3], 0.123999, 0.766222);
    PlayerTextDrawSetProportional(playerid, _@m_ptd[playerid][3], 1);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][3], 1);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][3], 0);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][3], -1);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][3], 2);

    _@m_ptd[playerid][4] = CreatePlayerTextDraw(playerid, 607.598144, 193.359298, #); // Page
    PlayerTextDrawLetterSize(playerid, _@m_ptd[playerid][4], 0.123999, 0.766222);
    PlayerTextDrawSetProportional(playerid, _@m_ptd[playerid][4], 1);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][4], 0);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][4], 3);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][4], -1);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][4], 2);
    
    format(player_menu_callback[playerid], 1, "");
    format(player_menu_callback_key[playerid], 1, "");
    for(new i = 0; i < MENU_MAX_ITEM_PER_PAGE; i ++) 
    {
        _@m_item[playerid][i] = PlayerText:-1;
        _@m_item2[playerid][i] = PlayerText:-1;
        _@m_box[playerid][i] = PlayerText:-1;
    }

    SetPVarInt(playerid, "Menu.SelectIndex", -1);
    return 1;
}

// INFO: gancho na OnPlayerDisconnect por destruir as textdraw em geral
hook OnPlayerDisconnect(playerid, reason)
{
    Iter_Clear(player_items[playerid]);
    for(new i = 0; i < 5; i ++) 
    {
        if(IsDynamicPlayerTextDrawVisible(playerid, _@m_ptd[playerid][i]))
            PlayerTextDrawDestroy(playerid, _@m_ptd[playerid][i]);
    }
    for(new i = 0; i < MENU_MAX_ITEM_PER_PAGE; i ++) 
    {
        if(_@m_item[playerid][i] != PlayerText:INVALID_TEXT_DRAW) 
        {
            if(IsDynamicPlayerTextDrawVisible(playerid, _@m_item[playerid][i]))
            {
                PlayerTextDrawDestroy(playerid, _@m_item[playerid][i]);
                _@m_item[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
            }
        }               
        if(_@m_item2[playerid][i] != PlayerText:INVALID_TEXT_DRAW) 
        {
            if(IsDynamicPlayerTextDrawVisible(playerid, _@m_item2[playerid][i]))
            {
                PlayerTextDrawDestroy(playerid, _@m_item2[playerid][i]);
                _@m_item2[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
            }
        }      
        if(_@m_box[playerid][i] != PlayerText:INVALID_TEXT_DRAW) 
        {
            if(IsDynamicPlayerTextDrawVisible(playerid, _@m_box[playerid][i]))
            {
                PlayerTextDrawDestroy(playerid, _@m_box[playerid][i]);
                _@m_box[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
            }
        }   
    }   
    return 1;
}

// INFO: gancho no OnPlayerUpdate para verificar se o player esta apertando as keys
// OBS: Não recomendo o uso do OnPlayerUpdate pois ela é chamada muitas vezes por segundo.
hook OnPlayerUpdate(playerid)
{
    if(Menu_Showing(playerid) && count_tick[playerid] - GetTickCount() <= 0)
    {
        if(!Menu_IsPlayerAndroid(playerid))
        {
            new 
                nextIndex, Keys, ud, lr,
                curPage = GetPVarInt(playerid, "Menu.curPage"),
                oldIndex = GetPVarInt(playerid, "Menu.SelectIndex");

        // nextIndex = oldIndex;
            GetPlayerKeys(playerid, Keys, ud, lr);
            #pragma unused Keys, lr
            switch(ud)
            {
                case KEY_DOWN:
                {
                    if(oldIndex + 1 >= MENU_MAX_ITEM_PER_PAGE) 
                    {
                        nextIndex = 0;
                        Menu_ShowPage(playerid, ++curPage >= Menu_GetMaxPage(playerid) ? 0 : curPage);
                    }
                    else 
                    {
                        nextIndex = oldIndex + 1;
                        if((MENU_MAX_ITEM_PER_PAGE * curPage + nextIndex) >= Iter_Count(player_items[playerid])) 
                        {
                            nextIndex = 0;
                            Menu_ShowPage(playerid, 0);
                        }
                    }
                    Menu_SelectIndex(playerid, nextIndex, oldIndex);    
                    CallLocalFunction(player_menu_callback_key[playerid], "ii", playerid, nextIndex);
                }
                case KEY_UP:
                {
                    if(oldIndex - 1 <= -1) 
                    {
                        new 
                            page;
                        
                        Menu_ShowPage(playerid, --curPage <= -1 ? Menu_GetMaxPage(playerid) - 1: curPage);
                        page = (curPage == -1) ? Menu_GetMaxPage(playerid) - 1 : GetPVarInt(playerid, "Menu.curPage");
                        nextIndex = Menu_GetMaxItemPerPage(playerid, page) - 1;
                        //  printf("<A> nextIndex=%d; curPage=%d;", nextIndex, curPage);
                    }
                    else 
                    {
                        nextIndex = oldIndex - 1;
                        if((MENU_MAX_ITEM_PER_PAGE * curPage + nextIndex) >= Iter_Count(player_items[playerid])) 
                        {
                            nextIndex = 0;
                            Menu_ShowPage(playerid, 0);
                        }
                        //  printf("<B> nextIndex=%d; curPage=%d;", nextIndex, curPage);
                    }
                    if(nextIndex <= -1)
                    {
                        new 
                            page = Menu_GetMaxPage(playerid) - 1;

                        Menu_ShowPage(playerid, page);
                        nextIndex = Menu_GetMaxItemPerPage(playerid, page - 1);
                        //  printf("Page=%d; nextIndex=%d;", page, nextIndex);
                    }
                    Menu_SelectIndex(playerid, nextIndex, oldIndex);    
                    CallLocalFunction(player_menu_callback_key[playerid], "ii", playerid, nextIndex);
                }
            }
        }
    }
    return 1;
}

// INFO: gancho para verificar se o jogador esta apertando keys
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!Menu_IsPlayerAndroid(playerid))
    {
        if(Menu_Showing(playerid))
        {
            switch(newkeys)
            {
                case KEY_SECONDARY_ATTACK, KEY_SPRINT: 
                { 
                    new 
                        response = (newkeys & KEY_SECONDARY_ATTACK) ? 1 : 0;
                    
                    Selecionar(playerid, response);
                }           
            }
        }
    }
    return 1;
}

// INFO: iniciar o iterator
hook OnGameModeInit()
{
    Iter_Init(player_items);
}

//
//    db    88b 88 8888b.  88""Yb  dP"Yb  88 8888b.  
//   dPYb   88Yb88  8I  Yb 88__dP dP   Yb 88  8I  Yb 
//  dP__Yb  88 Y88  8I  dY 88"Yb  Yb   dP 88  8I  dY 
// dP""""Yb 88  Y8 8888Y"  88  Yb  YbodP  88 8888Y"  
// 

// INFO: Verificar se o Player (Android) está apertando a textdraw
hook OnClickDynamicPlayerTD(playerid, PlayerText:playertextid)
{
    new 
        nextIndex,
        curPage = GetPVarInt(playerid, "Menu.curPage"),
        oldIndex = GetPVarInt(playerid, "Menu.SelectIndex");

    if(playertextid == SelecAndroid[playerid][0]) // cima
    {
        if(oldIndex - 1 <= -1) 
        {
            new 
                page;
            
            Menu_ShowPage(playerid, --curPage <= -1 ? Menu_GetMaxPage(playerid) - 1: curPage);
            page = (curPage == -1) ? Menu_GetMaxPage(playerid) - 1 : GetPVarInt(playerid, "Menu.curPage");
            nextIndex = Menu_GetMaxItemPerPage(playerid, page) - 1;
            //  printf("<A> nextIndex=%d; curPage=%d;", nextIndex, curPage);
        }
        else 
        {
            nextIndex = oldIndex - 1;
            if((MENU_MAX_ITEM_PER_PAGE * curPage + nextIndex) >= Iter_Count(player_items[playerid])) 
            {
                nextIndex = 0;
                Menu_ShowPage(playerid, 0);
            }
            //  printf("<B> nextIndex=%d; curPage=%d;", nextIndex, curPage);
        }
        if(nextIndex <= -1)
        {
            new 
                page = Menu_GetMaxPage(playerid) - 1;

            Menu_ShowPage(playerid, page);
            nextIndex = Menu_GetMaxItemPerPage(playerid, page - 1);
            //  printf("Page=%d; nextIndex=%d;", page, nextIndex);
        }
        Menu_SelectIndex(playerid, nextIndex, oldIndex);
        CallLocalFunction(player_menu_callback_key[playerid], "ii", playerid, nextIndex);
    }
    if(playertextid == SelecAndroid[playerid][1]) // baixo
    {
        if(oldIndex + 1 >= MENU_MAX_ITEM_PER_PAGE) 
        {
            nextIndex = 0;
            Menu_ShowPage(playerid, ++curPage >= Menu_GetMaxPage(playerid) ? 0 : curPage);
        }
        else 
        {
            nextIndex = oldIndex + 1;
            if((MENU_MAX_ITEM_PER_PAGE * curPage + nextIndex) >= Iter_Count(player_items[playerid])) 
            {
                nextIndex = 0;
                Menu_ShowPage(playerid, 0);
            }
        }
        Menu_SelectIndex(playerid, nextIndex, oldIndex);    
        CallLocalFunction(player_menu_callback_key[playerid], "ii", playerid, nextIndex);
    }
    if(playertextid == SelecAndroid[playerid][2]) // SELECIONAR
    {
        Selecionar(playerid, 1);
    }
    if(playertextid == SelecAndroid[playerid][3]) // sair
    {
        Selecionar(playerid, 0);
    }
    return 1;
}

// INFO: Auto Explicativo
stock Selecionar(playerid, response)
{
    if(strlen(player_menu_callback[playerid]) > 0) 
    {
        CallLocalFunction(player_menu_callback[playerid], "ddd", playerid, response, Menu_CurrentItem(playerid));
    }
}

// INFO: Aparecer o menu android
stock ShowMiniMenu(playerid)
{
    for(new i = 0; i < 4; ++i)
        if(!IsDynamicPlayerTextDrawVisible(playerid, SelecAndroid[playerid][i]))
            PlayerTextDrawShow(playerid, SelecAndroid[playerid][i]);
    SelectTextDraw(playerid, -1);
    return 1;
}

// INFO: Desaparecer o menu android
stock HideMiniMenu(playerid)
{
    for(new i = 0; i < 4; ++i)
        if(IsDynamicPlayerTextDrawVisible(playerid, SelecAndroid[playerid][i]))
            PlayerTextDrawHide(playerid, SelecAndroid[playerid][i]);
    CancelSelectTextDraw(playerid);
    return 1;
}