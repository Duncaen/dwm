/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 10;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "-*-terminus-medium-r-*-*-12-*-*-*-*-*-*-*" };
static const char dmenufont[]       = "-*-terminus-medium-r-*-*-12-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#9fefef";
static const char normbgcolor[]     = "#181818";
static const char normfgcolor[]     = "#D8D8D8";
static const char selbordercolor[]  = "#57a8a8";
static const char selbgcolor[]      = "#383838";
static const char selfgcolor[]      = "#D8D8D8";
static const char urgentcolor[]     = "#AB4642";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel] =  { selfgcolor, selbgcolor, selbordercolor },
	[SchemeUrg] =  { selfgcolor, urgentcolor, urgentcolor },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor  border */
	{ "termainl-pi",  NULL,       NULL,       1 << 0,       0,       -1,      1 },
	{ "terminal",     NULL,       NULL,       1 << 1,       0,       -1,      1 },
	{ "Firefox",      NULL,       NULL,       1 << 6,       0,       -1,      1 },
	{ "Chromium",     NULL,       NULL,       1 << 6,       0,       -1,      1 },
	{ "mpv",          NULL,       NULL,       1 << 4,       0,       -1,      1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int fresizehints = 1;   /* 1 means respect size hints in floated resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *sshcmd[] = { "dmenu_ssh", "-m", dmenumon, NULL };
static const char *termcmd[]  = { "st", "-c", "terminal", "-e", "tmux", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ControlMask,           XK_j,      movey,          {.i = +10 } },
	{ MODKEY|ControlMask,           XK_k,      movey,          {.i = -10 } },
	{ MODKEY|ControlMask|ShiftMask, XK_j,      movey,          {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_k,      movey,          {.i = -1 } },
	{ MODKEY|ControlMask,           XK_l,      movex,          {.i = +10 } },
	{ MODKEY|ControlMask,           XK_h,      movex,          {.i = -10 } },
	{ MODKEY|ControlMask|ShiftMask, XK_l,      movex,          {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_h,      movex,          {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_j,      resizeh,        {.i = +10 } },
	{ MODKEY|Mod1Mask,              XK_k,      resizeh,        {.i = -10 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_j,      resizeh,        {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_k,      resizeh,        {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_l,      resizew,        {.i = +10 } },
	{ MODKEY|Mod1Mask,              XK_h,      resizew,        {.i = -10 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_l,      resizew,        {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,      resizew,        {.i = -1 } },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = sshcmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
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
