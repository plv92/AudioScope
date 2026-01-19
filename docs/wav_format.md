# WAV File Format – AudioScope Reference

Ce document décrit la structure d’un fichier WAV et les notions audio nécessaires
à l’implémentation d’un analyseur audio simple (Peak, RMS, dBFS).

L’objectif est de comprendre **comment lire** un fichier WAV PCM et **comment interpréter**
correctement les données audio qu’il contient.

---

## 1. Qu’est-ce qu’un fichier WAV ?

Un fichier WAV est un **conteneur audio**, pas un codec.

- Il stocke de l’audio **non compressé** dans la majorité des cas
- Les données audio sont généralement au format **PCM**
- Il est basé sur le format **RIFF** (Resource Interchange File Format)

Un fichier WAV contient :
- des **métadonnées** (comment interpréter le son)
- des **données audio brutes** (les samples)

---

## 2. Structure globale d’un fichier WAV

Un fichier WAV est composé de plusieurs blocs appelés **chunks**.

Structure minimale d’un WAV PCM :

```

[ RIFF header ]
[ fmt  chunk ]
[ data chunk ]

```

Chaque chunk suit cette structure logique :

```

[ ChunkID (4 bytes) ][ ChunkSize (4 bytes) ][ ChunkData (ChunkSize bytes) ]

```

---

## 3. RIFF Header

Le RIFF header indique que le fichier est un WAV.

| Champ | Taille | Description |
|----|----|----|
| ChunkID | 4 bytes | Contient "RIFF" |
| ChunkSize | 4 bytes | Taille du fichier - 8 |
| Format | 4 bytes | Contient "WAVE" |

Exemple ASCII :
```

RIFF....WAVE

```

Si ces valeurs sont incorrectes, le fichier **n’est pas un WAV valide**.

---

## 4. fmt chunk (format audio)

Le chunk `fmt ` décrit **comment les données audio doivent être lues**.

### Structure du `fmt ` chunk (PCM)

| Champ | Taille | Description |
|----|----|----|
| Subchunk1ID | 4 bytes | "fmt " |
| Subchunk1Size | 4 bytes | 16 pour PCM |
| AudioFormat | 2 bytes | 1 = PCM |
| NumChannels | 2 bytes | 1 = mono, 2 = stéréo |
| SampleRate | 4 bytes | Ex: 44100 Hz |
| ByteRate | 4 bytes | Octets par seconde |
| BlockAlign | 2 bytes | Taille d’un frame audio |
| BitsPerSample | 2 bytes | 16, 24, 32… |

---

## 5. Définitions des notions audio

### Sample (échantillon)

Un **sample** est une valeur numérique représentant l’amplitude du signal audio
à un instant précis.

- L’audio analogique est continu
- L’audio numérique est une suite de samples discrets

---

### Sample Rate (fréquence d’échantillonnage)

Nombre de samples par seconde.

Exemples courants :
- 44100 Hz → CD
- 48000 Hz → vidéo

Un sample rate plus élevé = meilleure précision temporelle.

---

### NumChannels (nombre de canaux)

Nombre de flux audio simultanés :
- 1 → mono
- 2 → stéréo (gauche + droite)

Les samples sont **entrelacés** :
```

L R L R L R ...

```

---

### BitsPerSample (bit depth)

Nombre de bits utilisés pour représenter un sample.

| Bits | Plage de valeurs |
|----|----|
| 16 bits | -32768 à 32767 |
| 24 bits | plus grande dynamique |
| 32 bits | souvent float |

Impact :
- précision
- dynamique
- bruit de fond

---

### BlockAlign

Taille d’un **frame audio complet** (tous les canaux à un instant donné).

Formule :
```

BlockAlign = NumChannels × BitsPerSample / 8

```

Exemple :
- Stéréo 16 bits → 2 × 16 / 8 = 4 bytes

---

### ByteRate

Nombre d’octets lus par seconde.

Formule :
```

ByteRate = SampleRate × BlockAlign

```

---

### AudioFormat

Indique le type d’encodage audio :
- 1 → PCM (non compressé)
- autres valeurs → formats compressés

Dans AudioScope :
> Seul le format **PCM (AudioFormat = 1)** est supporté.

---

## 6. data chunk (données audio)

Le chunk `data` contient les **samples audio bruts**.

| Champ | Taille |
|----|----|
| Subchunk2ID | 4 bytes ("data") |
| Subchunk2Size | 4 bytes |
| Data | Subchunk2Size bytes |

### Nombre de samples

```

NumberOfSamples = Subchunk2Size / BlockAlign

```

---

## 7. Champs indispensables pour AudioScope

Pour analyser un fichier WAV PCM, AudioScope a besoin de :

| Champ | Utilité |
|----|----|
| SampleRate | Calcul de la durée |
| BitsPerSample | Conversion int → float |
| NumChannels | Organisation des samples |
| Subchunk2Size | Nombre total de samples |
| AudioFormat | Vérification du format |

---

## 8. Limitations acceptées du projet

Pour simplifier l’implémentation :

- Uniquement WAV PCM
- fmt chunk avant data chunk
- Pas de chunks optionnels (LIST, JUNK, etc.)
- Pas de formats compressés

Ces choix sont **acceptables** pour un projet étudiant s’ils sont documentés.

---

## 9. Résumé

Un fichier WAV est :
- un conteneur RIFF
- contenant des métadonnées audio
- et des samples PCM bruts

Comprendre le format WAV est indispensable pour :
- lire correctement les samples
- calculer Peak, RMS et dBFS
- éviter les erreurs d’interprétation audio

---