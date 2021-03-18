/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 0;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 0;       /* vert inner gap between windows */
static const unsigned int gappoh    = 0;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 0;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 30;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */

static const char col_bg[]       = "#073642";
static const char col_fg[]       = "#eee8d5";
static const char col_sl[]       = "#cb4b16";
static const char col_dd[]       = "#000000";
static const char *colors[][3]      = {
	/*                      fg         bg         border   */
	[SchemeNorm]        = { col_fg, col_bg, col_bg },
	[SchemeSel]         = { col_fg, col_sl, col_sl },

    [SchemeTagsNorm]    = { col_fg, col_bg, col_dd },
	[SchemeTagsSel]     = { col_fg, col_sl, col_dd },

    [SchemeInfoNorm]    = { col_fg, col_bg, col_dd },
    [SchemeInfoSel]     = { col_fg, col_bg, col_dd },

	[SchemeStatus]      = { col_fg, col_bg,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
};

static const char *fonts[] =
{
    "Fantasque Sans Mono:size=8",
    "Material Icons:size=10",
    "FontAwesome:size=10",
    "Noto Color Emoji:size=10"
};

static const char *const autostart[] = {
	"dunst", NULL,
	"picom", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
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
static const char *dmenucmd[] = { "rofi", "-drun", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *layoutmenu_cmd = "layoutmenu.sh";

/* commands spawned when clicking statusbar, the mouse button pressed is exported as BUTTON */
static char *statuscmds[] = { "notify-send Mouse$BUTTON", "notify-send hello" };
static char *statuscmd[] = { "/bin/sh", "-c", NULL, NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,   XK_Return,    spawn,          {.v = termcmd } },
	{ MODKEY,             XK_d,         spawn,          SHCMD("rofi -show drun")      },
	{ MODKEY|ShiftMask,   XK_s,         spawn,           SHCMD("flameshot gui")       },
	{ MODKEY|ShiftMask,   XK_q,         spawn,           SHCMD("manage-power.sh")       },

	{ MODKEY,             XK_b,         togglebar,      {0} },
	//{ MODKEY,             XK_s,         togglesticky,   {0} },
	//{ MODKEY,             XK_f,         togglefullscr,  {0} },
	//{ MODKEY,             XK_apostrophe, toggleAttachBelow,   {0} },

	{ MODKEY,             XK_minus,     incnmaster,     {.i = -1 } },
	{ MODKEY,             XK_equal,     incnmaster,     {.i = +1 } },

	{ MODKEY,             XK_j,         focusstack,     {.i = +1 } },
	{ MODKEY,             XK_k,         focusstack,     {.i = -1 } },
	{ MODKEY,             XK_h,         setmfact,       {.f = -0.05} },
	{ MODKEY,             XK_l,         setmfact,       {.f = +0.05} },

	{ MODKEY|ShiftMask,   XK_h,         setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,   XK_l,         setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,   XK_j,         movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,   XK_k,         movestack,      {.i = -1 } },

	{ MODKEY,             XK_t,         setlayout,      {.v = &layouts[0]} },
	{ MODKEY,             XK_m,         setlayout,      {.v = &layouts[1]} },
	{ MODKEY,             XK_g,         setlayout,      {.v = &layouts[7]} },

	{ MODKEY,             XK_0,         view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,   XK_0,         tag,            {.ui = ~0 } },

	{ MODKEY,             XK_q,         killclient,     {0} },
	{ MODKEY,             XK_Return,    zoom,           {0} },
	{ MODKEY|ShiftMask,   XK_e,         quit,           {0} },

	TAGKEYS(XK_1, 0)
	TAGKEYS(XK_2, 1)
	TAGKEYS(XK_3, 2)
	TAGKEYS(XK_4, 3)
	TAGKEYS(XK_5, 4)
	TAGKEYS(XK_6, 5)
	TAGKEYS(XK_7, 6)
	TAGKEYS(XK_8, 7)
	TAGKEYS(XK_9, 8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        layoutmenu,     {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = statuscmd } },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = statuscmd } },
	{ ClkStatusText,        0,              Button3,        spawn,          {.v = statuscmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

