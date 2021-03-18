static const unsigned int borderpx  = 4;
static const unsigned int snap      = 32;
static const int swallowfloating    = 0;
static const int showbar            = 1;
static const int topbar             = 1;
static const int user_bh            = 30;

static const unsigned int gappih    = 0;
static const unsigned int gappiv    = 0;
static const unsigned int gappoh    = 0;
static const unsigned int gappov    = 0;
static       int smartgaps          = 0;

static const char col_bg[]       = "#073642";
static const char col_fg[]       = "#eee8d5";
static const char col_sl[]       = "#cb4b16";
static const char col_dd[]       = "#000000";
static const char *colors[][3]      = {
	/*                      fg      bg      border */
	[SchemeNorm]        = { col_fg, col_bg, col_bg },
	[SchemeSel]         = { col_fg, col_sl, col_sl },

    [SchemeTagsNorm]    = { col_fg, col_bg, col_dd },
	[SchemeTagsSel]     = { col_fg, col_sl, col_dd },

    [SchemeInfoNorm]    = { col_fg, col_bg, col_dd },
    [SchemeInfoSel]     = { col_fg, col_bg, col_dd },

	[SchemeStatus]      = { col_fg, col_bg, col_dd },
};
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,4}, {2,3}, {0,4}, {0,0} }; /* represents the icon as an array of vertices like in grade school math */
static const XPoint stickyiconbb    = {4,4};	/* defines the bottom right corner of the bounding box of the polygon (origin is always 0,0) */
#define STICKYICONSH boxw + (boxw * 6 / 7) 	/* defines the height of the final, scaled polygon as it will be drawn. boxw is equal to the width of the shape */

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
	"dwmblocks", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { " 1 ", " 2 ", " 3 ", " 4 ", " 5 ", " 6 ", " 7 ", " 8 ", " 9 " };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Alacritty",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static int attachbelow = 0;    /* 1 means attach after the currently active window */

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

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,   XK_Return,    spawn,          {.v = termcmd } },
	{ MODKEY,             XK_d,         spawn,          SHCMD("rofi -show drun")            },
	{ MODKEY|ShiftMask,   XK_s,         spawn,          SHCMD("flameshot gui")              },
	{ MODKEY|ShiftMask,   XK_q,         spawn,          SHCMD("manage-power.sh")            },
	{ MODKEY,             XK_space,     spawn,          SHCMD("pkill -RTMIN+9 dwmblocks")   },

	{ MODKEY,             XK_b,         togglebar,      {0} },
	{ MODKEY,             XK_s,         togglesticky,   {0} },
	{ MODKEY,             XK_f,         togglefullscr,  {0} },
	{ MODKEY,             XK_apostrophe, toggleAttachBelow,   {0} },

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
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

