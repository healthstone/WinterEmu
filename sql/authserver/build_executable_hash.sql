DROP TABLE IF EXISTS auth_server.build_executable_hash;

CREATE TABLE auth_server.build_executable_hash
(
    build          int4  NOT NULL,
    platform       bpchar(4) NOT NULL,
    executablehash bytea NOT NULL,
    CONSTRAINT build_executable_hash_pkey PRIMARY KEY (build, platform)
);

INSERT INTO auth_server.build_executable_hash (build, platform, executablehash)
VALUES (5875, 'OSX ', decode('8D173CC381961EEBABF336F5E6675B101BB513E5', 'hex')),
       (5875, 'Win ', decode('95EDB27C7823B363CBDDAB56A392E7CB73FCCA20', 'hex')),
       (8606, 'OSX ', decode('D8B0ECFE534BC1131E19BAD1D4C0E813EEE4994F', 'hex')),
       (8606, 'Win ', decode('319AFAA3F2559682F9FF658BE01456255F456FB1', 'hex')),
       (12340, 'OSX ', decode('B706D13FF2F4018839729461E3F8A0E2B5FDC034', 'hex')),
       (12340, 'Win ', decode('CDCBBD5188315E6B4D19449D492DBCFAF156A347', 'hex'));