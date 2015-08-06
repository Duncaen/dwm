/* See LICENSE file for copyright and license details. */

/* appearance */
static const char *fonts[] = {
    "Terminess Powerline:style=Regular:size=9",
    "MaterialDesignIcons:size=9.2", // :antialias=false:hinting=false
};

#define normbordercolor "#383838"
#define normbgcolor     "#181818"
#define normfgcolor     "#D8D8D8"
#define selbordercolor  "#7CAFC2"
#define selbgcolor      "#383838"
#define selfgcolor      "#D8D8D8"

#define NUMCOLORS 15
static const char colors[NUMCOLORS][3][8] = {
    // border          foreground   background
    { normbordercolor, normfgcolor, normbgcolor }, // 1 normal
    { selbordercolor,  selfgcolor,  selbgcolor  }, // 2 selected
    { "#AB4642",       normfgcolor, "#AB4642"   }, // 3 urgent
    { normbordercolor, "#A1B56C",   normbgcolor }, // 4 normal green text
    { normbordercolor, "#AB4642",   normbgcolor }, // 5 normal red text
    { normbordercolor, "#F7CA88",   normbgcolor }, // 6 normal yellow text
    { selbordercolor,  "#A1B56C",   selbgcolor  }, // 7 selected green text
    { selbordercolor,  "#AB4642",   selbgcolor  }, // 8 selected red text
    { selbordercolor,  "#F7CA88",   selbgcolor  }, // 9 selected  yellow text
    { normbordercolor, normfgcolor, "#A1B56C"   }, // 10 normal green background
    { normbordercolor, normfgcolor, "#AB4642"   }, // 11 normal red background
    { normbordercolor, normfgcolor, "#F7CA88"   }, // 12 nromal yellow background
    { selbordercolor,  selfgcolor,  "#A1B56C"   }, // 13 normal green background
    { selbordercolor,  selfgcolor,  "#AB4642"   }, // 14 normal red background
    { selbordercolor,  selfgcolor,  "#F7CA88"   }  // 15 nromal yellow background
};

static const char dmenufont[] = "-*-terminus-medium-r-*-*-16-*-*-*-*-*-*-*";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

#define ICON_CONSOLE "\uF23D"
#define ICON_MUSIC "\uF3E3"
#define ICON_MAIL "\uF292"
#define ICON_BROWSER "\uF2CF"
#define ICON_VM "\uF26B"

static const char ICON_1[] = "\uF3F8";
#define ICON_2 "\uF3FD"
#define ICON_3 "\uF400"
#define ICON_4 "\uF403"
#define ICON_5 "\uF406"
#define ICON_6 "\uF409"
#define ICON_7 "\uF40C"
#define ICON_8 "\uF40F"
#define ICON_9 "\uF412"

#define ICON_TILE "\uF551"
#define ICON_FLOAT "\uF2D0"


/* tagging */
static const char *tags[] = {
	"[1]",
	"[2]",
	"[3]",
	"[4]",
	ICON_MUSIC " [5]",
	ICON_MAIL " [6]",
	ICON_BROWSER " [7]",
	ICON_VM " [8]",
	"[9]"
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance    title       tags mask     isfloating   monitor */
	{ "Gimp",         NULL,       NULL,       0,            True,        -1 },
	{ "Firefox",      NULL,       NULL,       1 << 6,       False,       -1 },
	{ "Chromium",     NULL,       NULL,       1 << 6,       False,       -1 },
	{ "Dwb",          NULL,       NULL,       1 << 6,       False,       -1 },
	{ "Thunderbird",  NULL,       NULL,       1 << 5,       False,       -1 },
	{ "Spotify",      NULL,       NULL,       1 << 4,       False,       -1 },
	{ "Tomahawk",     NULL,       NULL,       1 << 4,       False,       -1 },
	{ "Vlc",          NULL,       NULL,       1 << 4,       False,       -1 },
	{ "VirtualBox",   NULL,       NULL,       1 << 7,       False,       -1 },
	{ "VBoxSDL",      NULL,       NULL,       1 << 7,       False,       -1 },
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = True; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ ICON_TILE,      tile },    /* first entry is default */
	{ ICON_FLOAT,      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", /*"-m", dmenumon,*/ NULL };
static const char *playerctl[] = { "dmenu_playerctl", NULL };
static const char *powermenu[]  = { "dmenu_power", NULL };
static const char *termcmd[]  = { "st", "-e", "dvtm", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	// { MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY,                       XK_space,  spawn,          {.v = playerctl } },
	{ MODKEY,                       XK_Escape, spawn,          {.v = powermenu } },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

