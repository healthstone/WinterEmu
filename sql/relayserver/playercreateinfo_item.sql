DROP TABLE IF EXISTS relay_server.playercreateinfo_item;

CREATE TABLE relay_server.playercreateinfo_item
(
    race   SMALLINT NOT NULL DEFAULT 0,
    class  SMALLINT NOT NULL DEFAULT 0,
    itemid INTEGER  NOT NULL DEFAULT 0,
    amount SMALLINT NOT NULL DEFAULT 1,
    PRIMARY KEY (race, class, itemid)
);

CREATE INDEX playercreateinfo_race_class_index ON relay_server.playercreateinfo_item (race, class);

INSERT INTO relay_server.playercreateinfo_item (race, class, itemid, amount) VALUES(0, 6, 40582, -1);