CREATE TABLE Lieux(
ID INT PRIMARY KEY,
Nom TEXT,
Description TEXT,
);

CREATE TABLE Objets(
ID INT PRIMARY KEY,
Nom TEXT,
Description TEXT,
ID_lieu INT FOREIGN KEY,
);

CREATE TABLE Joueurs(
ID INT PRIMARY KEY,
Nom TEXT,
Vie INT,
Force INT,
Position_ID INT FOREIGN KEY,
);

CREATE TABLE Png(
ID INT PRIMARY KEY,
Nom TEXT,
Description TEXT,
ID_lieu INT FOREIGN KEY,
Dialogue TEXT,
);

CREATE TABLE Ennemis(
ID INT PRIMARY KEY,
Nom TEXT,
Vie INT,
Force INT,
ID_lieu INT FOREIGN KEY;
);

CREATE TABLE Inventaire(
ID INT PRIMARY KEY,
ID_joueur INT FOREIGN KEY,
ID_objet INT FOREIGN KEY,
);

CREATE TABLE Quetes(
ID INT PRIMARY KEY,
Description TEXT,
IsComplete BOOLEAN,
ID_joueur INT FOREIGN KEY,
ID_png INT FOREIGN KEY,
);

CREATE TABLE Png_quetes(
ID INT PRIMARY KEY,
ID_png INT FOREIGN KEY,
ID_quete INT FOREIGN KEY,
);

INSERT INTO Lieux(ID, Nom, Description)
VALUES
 (1, "Diddy Forest", "A forest that is known for it's abundance of baby oil. The oil is very useful, especially to make your ennemies slip. Beware of Diddy though..."),
 (2, "Drake's church", "A giant church with many evil childrens, who would be willing to do anything to defend the place. Be VERY careful if you ever decide to step in."),
 (3, "Los Pollos Hermanos", "Do NOT pick up items unless you are 100% sure that it's not a pipe bomb. The place is very explosive"),
 (4, "Chadi's shack", "Many people belive that this shack contains very odd objects, like the head tickler 2500");


INSERT INTO Objets(ID, Nom, Description, ID_lieu)
VALUES
(1, "Baby oil", "A very slippery substance that can stun ennemies by making them slip", 1),
(2, "Explosive fried gedagedigedagedago chicken nugget", "This friend chick nugget is very EXPLOSIVE! It deals alot of damage to ennemies", 3),
(3, "Head tickler 2500", "With this spooky ahh object, you can tickle the head of your ennemies. They will lose health by laughing too much.", 4),
(4, "Drake's slipper", "This object deals a small amount of damage but attacks extremly fast.", 2),
(5, "Chadi's transportable bed", "You can only use this bed once. With this, you can become immune for 1 turn by sleeping. It will heal 20% of your health.", 4);

INSERT INTO Ennemis(ID, Nom, Vie, Force, ID_lieu)
VALUES 
(1, "Diddy", "He deals ALOT of damage. Run away while you can. Do NOT fight him.", 1),
(2, "Drake", "He wont attack you if you are a child. However, if you are over 18, he will try to kick you out or kill you. Make sure you are strong enough.", 2),
(3, "Zombie", "This zombie is a normal entity. Do not get bitten or you will have to cure it with baby oil.", 5),
(4, "Deformed glue monster", "Located in Chadi's shack, this monster will try to submerge you in glue. However, he is very slow.", 4);