/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static unsigned int borderpx  = 2; /* border pixel of windows */
static unsigned int gappx     = 6; /* gaps between windows */
static unsigned int snap      = 0; /* snap pixel */
static int showbar            = 1; /* 0 means no bar */
static int topbar             = 1; /* 0 means bottom bar */
static char font[]            = "iosevka:pixelsize=12:antialias=true:autohint=true";
static const char *fonts[]    = { font };
static char normbgcolor[]     = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[]     = "#bbbbbb";
static char selfgcolor[]      = "#eeeeee";
static char selbordercolor[]  = "#005577";
static char selbgcolor[]      = "#005577";
static char *colors[][3] = {
	/*               fg             bg             border */
	[SchemeNorm] = { normfgcolor,   normbgcolor,   normbordercolor },
	[SchemeSel]  = { selfgcolor,    selbgcolor,    selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class   instance   title   tags mask   isfloating   monitor */
	{ NULL,    NULL,      NULL,   0,          0,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol   arrange function */
	{ "[]=",    tile    }, /* first entry is default */
	{ "><>",    NULL    }, /* no layout function means floating behavior */
	{ "[M]",    monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                         KEY,   view,         { .ui = 1 << TAG } }, \
	{ MODKEY|ControlMask,             KEY,   toggleview,   { .ui = 1 << TAG } }, \
	{ MODKEY|ShiftMask,               KEY,   tag,          { .ui = 1 << TAG } }, \
	{ MODKEY|ControlMask|ShiftMask,   KEY,   toggletag,    { .ui = 1 << TAG } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2]                      = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]                = { "dmenu_run", "-p", "Exec", "-i", "-m", dmenumon, NULL };
static const char *passmenucmd[]             = { "passmenu", "-p", "Pass", "-i", NULL };
static const char *passotpmenucmd[]          = { "passotpmenu", "-p", "OTP", "-i", NULL };
static const char *networkmanagerdmenucmd[]  = { "networkmanager_dmenu", NULL };
static const char *screenlockcmd[]           = { "slock", NULL };
static const char *terminalcmd[]             = { "st", NULL };
static const char *screenshotcmd[]           = { "screenshot", NULL };
static const char *switchkeyboardlayoutcmd[] = { "switch_keyboard_layout", NULL };
static const char *blueberrycmd[]            = { "blueberry", NULL };
static const char *pavucontrolcmd[]          = { "pavucontrol", NULL };
static const char *bravecmd[]                = { "brave", NULL };
static const char *firefoxcmd[]              = { "firefox", NULL };
static const char *discordcmd[]              = { "discord", NULL };
static const char *elementcmd[]              = { "element-desktop", NULL };
static const char *telegramcmd[]             = { "telegram-desktop", NULL };
static const char *intellijcmd[]             = { "idea", NULL };
static const char *volumeupcmd[]             = { "volume_up", NULL };
static const char *volumedowncmd[]           = { "volume_down", NULL };
static const char *volumetogglecmd[]         = { "volume_toggle", NULL };
static const char *microphonetogglecmd[]     = { "microphone_toggle", NULL };
static const char *brightnessupcmd[]         = { "brightnessctl", "set", "5%+", NULL };
static const char *brightnessdowncmd[]       = { "brightnessctl", "set", "5%-", NULL };
static const char *dunstctlclosecmd[]        = { "dunstctl", "close", NULL };
static const char *dunstctlhistorypopcmd[]   = { "dunstctl", "history-pop", NULL };
static const char *dunstctlcloseallcmd[]     = { "dunstctl", "close-all", NULL };
static const char *dunstctltogglecmd[]       = { "dunstctl", "set-paused", "toggle", NULL };

static Key keys[] = {
	/* modifier                       key                         function          argument */
	/* dmenu prompts */
	{ MODKEY,                         XK_p,                       spawn,            { .v = dmenucmd               }  },
	{ MODKEY|ShiftMask,               XK_p,                       spawn,            { .v = passmenucmd            }  },
	{ MODKEY,                         XK_o,                       spawn,            { .v = passotpmenucmd         }  },
	{ MODKEY,                         XK_n,                       spawn,            { .v = networkmanagerdmenucmd }  },
	/* start program */
	{ MODKEY,                         XK_space,                   spawn,            { .v = screenlockcmd           } },
	{ MODKEY,                         XK_Return,                  spawn,            { .v = terminalcmd             } },
	{ MODKEY,                         XK_s,                       spawn,            { .v = screenshotcmd           } },
	{ MODKEY|ShiftMask,               XK_space,                   spawn,            { .v = switchkeyboardlayoutcmd } },
	{ MODKEY|ShiftMask,               XK_b,                       spawn,            { .v = blueberrycmd            } },
	{ MODKEY|ShiftMask,               XK_v,                       spawn,            { .v = pavucontrolcmd          } },
	{ MODKEY|ShiftMask,               XK_w,                       spawn,            { .v = bravecmd                } },
	{ MODKEY|ShiftMask,               XK_f,                       spawn,            { .v = firefoxcmd              } },
	{ MODKEY|ShiftMask,               XK_e,                       spawn,            { .v = elementcmd              } },
	{ MODKEY|ShiftMask,               XK_t,                       spawn,            { .v = telegramcmd             } },
	{ MODKEY|ShiftMask,               XK_d,                       spawn,            { .v = discordcmd              } },
	{ MODKEY|ShiftMask,               XK_i,                       spawn,            { .v = intellijcmd             } },
	/* quit program */
	{ MODKEY|ShiftMask,               XK_c,                       killclient,       { 0 }                            },
	/* quit dwm */
	{ MODKEY|ShiftMask,               XK_q,                       quit,             { 0 }                            },
	/* notifications */
	{ MODKEY,                         XK_y,                       spawn,            { .v = dunstctlclosecmd      }   },
	{ MODKEY|ShiftMask,               XK_y,                       spawn,            { .v = dunstctlhistorypopcmd }   },
	{ MODKEY|ControlMask,             XK_y,                       spawn,            { .v = dunstctlcloseallcmd   }   },
	{ MODKEY|ControlMask|ShiftMask,   XK_y,                       spawn,            { .v = dunstctltogglecmd     }   },
	/* media keys */
	{ 0,                              XF86XK_AudioRaiseVolume,    spawn,            { .v = volumeupcmd         }     },
	{ 0,                              XF86XK_AudioLowerVolume,    spawn,            { .v = volumedowncmd       }     },
	{ 0,                              XF86XK_AudioMute,           spawn,            { .v = volumetogglecmd     }     },
	{ MODKEY|ShiftMask,               XK_a,                       spawn,            { .v = volumetogglecmd     }     },
	{ 0,                              XF86XK_AudioMicMute,        spawn,            { .v = microphonetogglecmd }     },
	{ MODKEY|ShiftMask,               XK_m,                       spawn,            { .v = microphonetogglecmd }     },
	{ 0,                              XF86XK_MonBrightnessUp,     spawn,            { .v = brightnessupcmd     }     },
	{ 0,                              XF86XK_MonBrightnessDown,   spawn,            { .v = brightnessdowncmd   }     },
	/* toggle bar */
	{ MODKEY,                         XK_b,                       togglebar,        { 0 }                            },
	/* change layout */
	{ MODKEY,                         XK_t,                       setlayout,        { .v = &layouts[0] }             },
	{ MODKEY,                         XK_f,                       setlayout,        { .v = &layouts[1] }             },
	{ MODKEY,                         XK_m,                       setlayout,        { .v = &layouts[2] }             },
//	{ MODKEY|ControlMask,             XK_space,                   setlayout,        { 0 }                            },
	{ MODKEY|ControlMask,             XK_space,                   togglefloating,   { 0 }                            },
	{ MODKEY|ControlMask,             XK_Return,                  togglefullscr,    { 0 }                            },
	/* resize windows */
	{ MODKEY,                         XK_h,                       setmfact,         { .f = -0.05 }                   },
	{ MODKEY,                         XK_l,                       setmfact,         { .f = +0.05 }                   },
	{ MODKEY,                         XK_o,                       setmfact,         { .f =  0.00 }                   },
	{ MODKEY|ShiftMask,               XK_l,                       setcfact,         { .f = -0.25 }                   },
	{ MODKEY|ShiftMask,               XK_h,                       setcfact,         { .f = +0.25 }                   },
	{ MODKEY|ShiftMask,               XK_o,                       setcfact,         { .f =  0.00 }                   },
	/* reorder windows */
	{ MODKEY,                         XK_i,                       incnmaster,       { .i = +1 }                      },
	{ MODKEY,                         XK_d,                       incnmaster,       { .i = -1 }                      },
	{ MODKEY|ShiftMask,               XK_Return,                  zoom,             { 0 }                            },
	/* navigate windows */
	{ MODKEY,                         XK_j,                       focusstack,       { .i = +1 }                      },
	{ MODKEY,                         XK_k,                       focusstack,       { .i = -1 }                      },
	/* navigate monitors */
	{ MODKEY,                         XK_comma,                   focusmon,         { .i = -1 }                      },
	{ MODKEY,                         XK_period,                  focusmon,         { .i = +1 }                      },
	/* change monitors */
	{ MODKEY|ShiftMask,               XK_comma,                   tagmon,           { .i = -1 }                      },
	{ MODKEY|ShiftMask,               XK_period,                  tagmon,           { .i = +1 }                      },
	/* change views */
	{ MODKEY,                         XK_Tab,                     view,             { 0 }                            },
	{ MODKEY,                         XK_0,                       view,             { .ui = ~0 }                     },
	/* change tags */
	{ MODKEY|ShiftMask,               XK_0,                       tag,              { .ui = ~0 }                     },
	TAGKEYS(                          XK_1,                       0 )
	TAGKEYS(                          XK_2,                       1 )
	TAGKEYS(                          XK_3,                       2 )
	TAGKEYS(                          XK_4,                       3 )
	TAGKEYS(                          XK_5,                       4 )
	TAGKEYS(                          XK_6,                       5 )
	TAGKEYS(                          XK_7,                       6 )
	TAGKEYS(                          XK_8,                       7 )
	TAGKEYS(                          XK_9,                       8 )
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click           event mask   button     function          argument */
	{ ClkLtSymbol,     0,           Button1,   setlayout,        { 0 }                },
	{ ClkLtSymbol,     0,           Button3,   setlayout,        { .v = &layouts[2] } },
	{ ClkWinTitle,     0,           Button2,   zoom,             { 0 }                },
	{ ClkStatusText,   0,           Button2,   spawn,            { .v = terminalcmd } },
	{ ClkClientWin,    MODKEY,      Button1,   movemouse,        { 0 }                },
	{ ClkClientWin,    MODKEY,      Button2,   togglefloating,   { 0 }                },
	{ ClkClientWin,    MODKEY,      Button3,   resizemouse,      { 0 }                },
	{ ClkTagBar,       0,           Button1,   view,             { 0 }                },
	{ ClkTagBar,       0,           Button3,   toggleview,       { 0 }                },
	{ ClkTagBar,       MODKEY,      Button1,   tag,              { 0 }                },
	{ ClkTagBar,       MODKEY,      Button3,   toggletag,        { 0 }                },
};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
	{ "normbgcolor",       STRING,    &normbgcolor     },
	{ "normbordercolor",   STRING,    &normbordercolor },
	{ "normfgcolor",       STRING,    &normfgcolor     },
	{ "selbgcolor",        STRING,    &selbgcolor      },
	{ "selbordercolor",    STRING,    &selbordercolor  },
	{ "selfgcolor",        STRING,    &selfgcolor      },
	{ "font",              STRING,    &font            },
	{ "borderpx",          INTEGER,   &borderpx        },
	{ "gappx",             INTEGER,   &gappx           },
	{ "snap",              INTEGER,   &snap            },
	{ "showbar",           INTEGER,   &showbar         },
	{ "topbar",            INTEGER,   &topbar          },
	{ "nmaster",           INTEGER,   &nmaster         },
	{ "resizehints",       INTEGER,   &resizehints     },
	{ "mfact",             FLOAT,     &mfact           },
};
