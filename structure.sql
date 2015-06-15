DROP TABLE IF EXISTS "archive";
CREATE TABLE "archive" ("best_user" VARCHAR NOT NULL , "data" DATETIME NOT NULL  DEFAULT CURRENT_DATE, "ochki" INTEGER NOT NULL  DEFAULT 0, PRIMARY KEY ("data"));
DROP TABLE IF EXISTS "frazy";
CREATE TABLE "frazy" ("id" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "name" VARCHAR NOT NULL );
INSERT INTO "frazy" VALUES(1,', игра уже запущена! ');
INSERT INTO "frazy" VALUES(2,' а игру никто и не запускал... ');
INSERT INTO "frazy" VALUES(3,'  Внимание, вопрос ');
INSERT INTO "frazy" VALUES(4,' :::==-- :tzar:');
INSERT INTO "frazy" VALUES(5,' балл(а,ов), ');
INSERT INTO "frazy" VALUES(6,'-е место.');
INSERT INTO "frazy" VALUES(7,' останавливает викторину!');
INSERT INTO "frazy" VALUES(8,' эта команда только для админов. ');
INSERT INTO "frazy" VALUES(9,':tzar: --==::: TOP ');
INSERT INTO "frazy" VALUES(10,'Подсказка ');
INSERT INTO "frazy" VALUES(11,' балл(а,ов) за команду !след.');
INSERT INTO "frazy" VALUES(12,'  Следующий вопрос ');
INSERT INTO "frazy" VALUES(13,'Никто не ответил правильно.');
INSERT INTO "frazy" VALUES(14,' запускает игру! ');
INSERT INTO "frazy" VALUES(15,'Никто не играет. Викторина автоматичеки завершена.');
INSERT INTO "frazy" VALUES(16,' игроков изъявили желание играть. Необходимо ');
INSERT INTO "frazy" VALUES(17,'Время ответа ');
INSERT INTO "frazy" VALUES(18,' умнейший(ая) в сети!');
INSERT INTO "frazy" VALUES(19,' получает звезду! ');
INSERT INTO "frazy" VALUES(20,' Правильный ответ: ');
INSERT INTO "frazy" VALUES(21,' игрока(ов) для старта игры. ');
INSERT INTO "frazy" VALUES(22,' ---------- ');
INSERT INTO "frazy" VALUES(23,'  У ');
INSERT INTO "frazy" VALUES(24,'№');
INSERT INTO "frazy" VALUES(25,'букв: ');
INSERT INTO "frazy" VALUES(26,'Среднее время ответа, сек: ');
INSERT INTO "frazy" VALUES(27,'Статистика пользователя ');
INSERT INTO "frazy" VALUES(28,'Баллы: ');
INSERT INTO "frazy" VALUES(29,'Звёзды: ');
INSERT INTO "frazy" VALUES(30,'Слов отправлено: ');
INSERT INTO "frazy" VALUES(31,'Засчитано: ');
INSERT INTO "frazy" VALUES(32,'Место: ');
INSERT INTO "frazy" VALUES(33,'Администратор: ');
INSERT INTO "frazy" VALUES(34,'Вопросов в базе: ');
INSERT INTO "frazy" VALUES(35,'Задано вопросов: ');
INSERT INTO "frazy" VALUES(36,' моментально угадал(а) и получил(а) ');
INSERT INTO "frazy" VALUES(37,' немного подумав, угадал(а) и заработал(а) ');
INSERT INTO "frazy" VALUES(38,' ответил(а) правильно и заработал(а) ');
INSERT INTO "frazy" VALUES(39,' скрипя мозгами, ответил(а) правильно и получил(а) ');
INSERT INTO "frazy" VALUES(40,' из последних сил угадал(а) и получил(а) ');
INSERT INTO "frazy" VALUES(41,' ты почти угадал(а)! ');
INSERT INTO "frazy" VALUES(42,' Игра викторина');
INSERT INTO "frazy" VALUES(43,'==============================================');
INSERT INTO "frazy" VALUES(44,'Лучшие в нашей сети:');
INSERT INTO "frazy" VALUES(45,'Интеллект: ');
INSERT INTO "frazy" VALUES(46,'Таблица обновлялась ');
INSERT INTO "frazy" VALUES(47,'Ранг: ');
INSERT INTO "frazy" VALUES(48,'Статистика обнулилась.');
INSERT INTO "frazy" VALUES(49,' сначала останови игру.');
INSERT INTO "frazy" VALUES(50,'[url]Команды в игре:[/url]
[url]!старт[/url] - запуск игры
[url]!супер[/url] - запуск суперигры
[url]!стоп[/url] - остановка игры
[url]!след[/url] - пропустить вопрос
[url]!стат[/url] - личная статистика
[url]!стат <имя пользователя>[/url] - статистика пользователя
[url]!топ[/url]  - топ 10 лучших в канал
[url]!топ <N>[/url] - топ N лучших (не более 200) в приват
[url]!сброс[/url] - очистить всю статистику (для админов)
[url]!список[/url] - список играющих
[url]!инфо[/url] - информация о плагине
[url]!команды[/url] - список команд
');
INSERT INTO "frazy" VALUES(51,' запускает супер игру! Вопрос будет без подсказок.');
INSERT INTO "frazy" VALUES(52,'Запуск супер-игры. Следующий вопрос будет без подсказок.');
INSERT INTO "frazy" VALUES(53,'Вы превысили лимит ответов на этот вопрос.');
INSERT INTO "frazy" VALUES(54,'Система бонусов включена');
INSERT INTO "frazy" VALUES(55,'Система бонусов отключена');
INSERT INTO "frazy" VALUES(60,'%user% %genansw% %+% %bonus% балл(а,ов), У %user% %points% балл(а,ов) %place%-е место.');
INSERT INTO "frazy" VALUES(61,'%user%> -%minus% балл(а,ов) за команду !след. %points% балл(а,ов), %place%-е место.');
INSERT INTO "frazy" VALUES(62,'%count% игроков изъявили желание играть. Необходимо %needcount% игроков для старта игры. ');
INSERT INTO "frazy" VALUES(63,'  Внимание, вопрос [url]№%№%[/url]:

  [url]%question% (букв: %letters%)[/url]
');
INSERT INTO "frazy" VALUES(64,'  Следующий вопрос [url]№%№%[/url]:

  [url]%question% (букв: %letters%)[/url]
');
INSERT INTO "frazy" VALUES(65,'  Внимание, вопрос [url]№%№%[/url]:

  [url]%question% (букв: %letters%, слов: %words%)[/url]
');
INSERT INTO "frazy" VALUES(66,'  Следующий вопрос [url]№%№%[/url]:

  [url]%question% (букв: %letters%, слов: %words%)[/url]
');
INSERT INTO "frazy" VALUES(68,'%user%>  Эта команда только для админов!');
INSERT INTO "frazy" VALUES(70,'Подсказка [url]№%№%[/url]: %help%');
DROP TABLE IF EXISTS "rangs";
CREATE TABLE "rangs" ("ochki" INTEGER NOT NULL  UNIQUE , "name" VARCHAR NOT NULL  UNIQUE );
DROP TABLE IF EXISTS "statistic";
CREATE TABLE "statistic" ("zadano" INTEGER NOT NULL  DEFAULT 0, "otgadano" INTEGER NOT NULL  DEFAULT 0);
DROP TABLE IF EXISTS "users";
CREATE TABLE "users" ("name" VARCHAR   PRIMARY KEY  NOT NULL ,"vsego" INTEGER NOT NULL  DEFAULT 0 ,"otgadal" INTEGER NOT NULL  DEFAULT 0 , "ochki" INTEGER NOT NULL  DEFAULT 0, "otvet_time" INTEGER NOT NULL  DEFAULT 0, "replies" INTEGER NOT NULL  DEFAULT 0 ,"in_game" INTEGER NOT NULL  DEFAULT 0 , "want_game" INTEGER NOT NULL  DEFAULT 0 , "last_answer" INTEGER NOT NULL  DEFAULT 0 , "isadmin" INTEGER NOT NULL  DEFAULT 0);
CREATE INDEX "users_ochki" ON "users" ("ochki" ASC);
