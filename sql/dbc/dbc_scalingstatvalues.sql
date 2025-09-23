DROP TABLE IF EXISTS dbc.dbc_scalingstatvalues;

CREATE TABLE dbc.dbc_scalingstatvalues
(
    id                   integer PRIMARY KEY DEFAULT 0,
    charlevel            integer NOT NULL    DEFAULT 0,
    shoulderbudget       integer NOT NULL    DEFAULT 0,
    trinketbudget        integer NOT NULL    DEFAULT 0,
    weaponbudget1h       integer NOT NULL    DEFAULT 0,
    rangedbudget         integer NOT NULL    DEFAULT 0,
    clothshoulderarmor   integer NOT NULL    DEFAULT 0,
    leathershoulderarmor integer NOT NULL    DEFAULT 0,
    mailshoulderarmor    integer NOT NULL    DEFAULT 0,
    plateshoulderarmor   integer NOT NULL    DEFAULT 0,
    weapondps1h          integer NOT NULL    DEFAULT 0,
    weapondps2h          integer NOT NULL    DEFAULT 0,
    spellcasterdps1h     integer NOT NULL    DEFAULT 0,
    spellcasterdps2h     integer NOT NULL    DEFAULT 0,
    rangeddps            integer NOT NULL    DEFAULT 0,
    wanddps              integer NOT NULL    DEFAULT 0,
    spellpower           integer NOT NULL    DEFAULT 0,
    primarybudget        integer NOT NULL    DEFAULT 0,
    tertiarybudget       integer NOT NULL    DEFAULT 0,
    clothcloakarmor      integer NOT NULL    DEFAULT 0,
    clothchestarmor      integer NOT NULL    DEFAULT 0,
    leatherchestarmor    integer NOT NULL    DEFAULT 0,
    mailchestarmor       integer NOT NULL    DEFAULT 0,
    platechestarmor      integer NOT NULL    DEFAULT 0
);
