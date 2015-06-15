ATTACH DATABASE 'путь_до_базы\old.db' as old ;
DELETE FROM users ;
INSERT INTO users  (name, vsego, otgadal, ochki, otvet_time)
SELECT name, vsego, otgadal, ochki, otvet_time  FROM old.users ;
DELETE FROM statistic ;
INSERT INTO statistic SELECT zadano, otgadano FROM old.statistic LIMIT 1 ;
