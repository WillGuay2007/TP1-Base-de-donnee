DROP TABLE IF EXISTS Png_quetes;
DROP TABLE IF EXISTS Quetes;
DROP TABLE IF EXISTS Inventaire;
DROP TABLE IF EXISTS Ennemis;
DROP TABLE IF EXISTS Png;
DROP TABLE IF EXISTS Joueurs;
DROP TABLE IF EXISTS Objets;
DROP TABLE IF EXISTS Lieux;

CREATE TABLE Lieux(
    ID INT PRIMARY KEY,
    Nom TEXT,
    Description TEXT
);

CREATE TABLE Objets(
    ID INT PRIMARY KEY,
    Nom TEXT,
    Description TEXT,
    ForAttack BOOLEAN DEFAULT 0,
    ForHeal BOOLEAN DEFAULT 0,
    AttackOrHealPercentage INT DEFAULT 0,
    ID_lieu INT,
    FOREIGN KEY(ID_lieu) REFERENCES Lieux(ID)
);

CREATE TABLE Joueurs(
    ID INT PRIMARY KEY,
    Nom TEXT,
    Vie INT,
    Force INT,
    Position_ID INT,
    FOREIGN KEY(Position_ID) REFERENCES Lieux(ID)
);

CREATE TABLE Png(
    ID INT PRIMARY KEY,
    Nom TEXT,
    Description TEXT,
    ID_lieu INT,
    Dialogue TEXT,
    FOREIGN KEY(ID_lieu) REFERENCES Lieux(ID)
);

CREATE TABLE Ennemis(
    ID INT PRIMARY KEY,
    Nom TEXT,
    Vie INT,
    Force INT,
    ID_lieu INT,
    FOREIGN KEY(ID_lieu) REFERENCES Lieux(ID)
);

CREATE TABLE Inventaire(
    ID INT PRIMARY KEY,
    ID_joueur INT,
    ID_objet INT,
    FOREIGN KEY(ID_joueur) REFERENCES Joueurs(ID),
    FOREIGN KEY(ID_objet) REFERENCES Objets(ID)
);

CREATE TABLE Quetes(
    ID INT PRIMARY KEY,
    Description TEXT,
    IsComplete BOOLEAN,
    ID_joueur INT,
    ID_png INT,
    FOREIGN KEY(ID_joueur) REFERENCES Joueurs(ID),
    FOREIGN KEY(ID_png) REFERENCES Png(ID)
);

CREATE TABLE Png_quetes(
    ID INT PRIMARY KEY,
    ID_png INT,
    ID_quete INT,
    FOREIGN KEY(ID_png) REFERENCES Png(ID),
    FOREIGN KEY(ID_quete) REFERENCES Quetes(ID)
);

INSERT INTO Lieux(ID, Nom, Description)
VALUES
 (1, "Diddy Forest", "A forest that is known for it's abundance of baby oil. The oil is very useful, especially to make your ennemies slip. Beware of Diddy though..."),
 (2, "Drake's church", "A giant church with many evil childrens, who would be willing to do anything to defend the place. Be VERY careful if you ever decide to step in."),
 (3, "Los Pollos Hermanos", "Do NOT pick up items unless you are 100% sure that it's not a pipe bomb. The place is very explosive"),
 (4, "Chadi's shack", "Many people belive that this shack contains very odd objects, like the head tickler 2500");


INSERT INTO Objets(ID, Nom, Description, ForAttack, ForHeal, AttackOrHealPercentage, ID_lieu)
VALUES
(1, "Baby oil", "Very tender to the skin... It heal 20% of your health", 0, 1, 20, 1),
(2, "Explosive fried gedagedigedagedago chicken nugget", "This friend chick nugget is very EXPLOSIVE! It deals alot of damage to ennemies (+25% force for 1 turn)", 1, 0, 25, 3),
(3, "Head tickler 2500", "With this spooky ahh object, you can tickle the head of your ennemies. They will lose health by laughing too much. (+15% force for 1 turn)", 1, 0, 15, 4),
(4, "Drake's slipper", "This object deals a small amount of damage but attacks extremly fast.(+10% force for 1 turn)", 1, 0, 10, 2),
(5, "Chadi's transportable bed", "You feel sleepy. Sleep is good for health, so it will heal 20% of your health.",0, 1, 20, 4);

INSERT INTO Ennemis(ID, Nom, Vie, Force, ID_lieu)
VALUES 
(1, "Diddy", 100, 50, 1),
(2, "Drake", 80, 40, 2),
(3, "Zombie", 60, 30, 1),
(4, "Deformed glue monster", 120, 20, 4),
(5, "Bombardino Chickeniriedo", 40, 40, 3);