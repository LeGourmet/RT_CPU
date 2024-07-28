# RT_CPU

## Présentation

Ceci est une version ameliorée d'un projet de lancer de rayons sur CPU réalisé durant ma première année du master ISICG. Le projet consistait à créer de toute pièce un moteur de rendu offline basé sur du lancer de rayons.

Le moteur est capable de gérer différents types de géométrie :
- Analytique : Plan, Sphère, Triangle et Triangle Mesh
- Implicite 3D : Plan, Sphère, Cube, Cylindre, Tore
- Fractale 3D : Appolonius, Julia set, MandelBulb, Menger sponge et Sierpinki triangle
- Paramétrique :

Le moteur dipose de différents types de lumière :
- Les basiques : Directionel et Ponctuel
- Les planaires : Disque et Quadrilataire
- Les volumique :

Le moteur intègre différents type de caméra :
- Projection : Perspective et Orthographique
- ...

Les materiaux des objets comportes ces paramètres :
- Albedo : 3 canaux rgb et 1 pour l'alpha
- AlphaCutoff : 0 pour opaque, 1 pour blend et le reste pour masque
- EmissiveColor : 3 canaux rgb
- EmissiveStrength : facteur de luminosité de l'objet entre 0 et plus l'infini
- Metalness : facteur entre 0 et 1
- Roughness : facteur entre 0 et 1
- Transmitness : facteur entre 0 et 1
- AbsorptionColor : 3 canaux rgb
- AbsorptionDensity : facteur entre 0 et plus l'infini qui correspond à l'atenuation distance
- ior : facteur différent de zéro determinant la capacité réfléchissante d'un matériaux diéléctrique et la quantité de déviation de la lumière au seins d'un matériaux transparent   

La méthode de rendu utilisé par le moteur est un path tracer avec Next Event Estimation pour réaliser un echantillonage direct des lumières de la scène.
Il est possible de régler le nombre de rayon par pixel et le nombre de rebond maximum de la lumière (1 correspondra à une illumination direct uniquement).

D'autres améliorations ont aussi été réalisées comme : 
- La création de structures accélératrices (AABB et BVH) qui ont pour vocation de grandement accélérer les calculs
- Stratified Sampling des lumières pour le calcule des rayons d'ombrage
- Stratified Sampling des rayons primaires
- Importance sampling de la BSDF
- Multiple importance sampling des PDFs de la BSDF et des lumière lors de l'évaluation du Next Event Estimation 

## Résultats


| Direct lighting | Path tracing  |
| - | - |
| ![Direct](results/conferenceDirect.png) | ![Indirect](results/conferenceIndirect.png) |

| Mandelbulb and julia set | Appolonius |
| - | - |
| ![Fractal1](results/fractal.png) | ![Fractal2](results/appolonius.png) |

| Roughness | Dielectric | Metalic | Transmit |
| - | - | - | - |
| 0 | ![Dielectric0](results/materials/dielectric0.png) | ![Metalic0](results/materials/metalic0.png) | ![Transmit0](results/materials/transmit0.png) |
| 0.3 | ![Dielectric1](results/materials/dielectric03.png) | ![Metalic1](results/materials/metalic03.png) | ![Transmit1](results/materials/transmit03.png) |
| 0.5 | ![Dielectric2](results/materials/dielectric05.png) | ![Metalic2](results/materials/metalic05.png) | ![Transmit2](results/materials/transmit05.png) |

# Future ajouts

Plusieurs ajouts sont en vue pour la géométrie dans ce moteur :
- l'ajout de nouveau type de géométrie analytique : landskape, micro-mesh, text
- le refactor du type de géométrie analytique "Triangle" pour permettre de géré la subdivition de surface
- la gestion de géométrie paramétrique (curves) : Bezier, NURBS, Catmull-Rom
- la gestion de géométrie paramétrique (surface) : NURBS
- le refactor des type de géométrie implicite "plane", "sphere", "torus", "box" et "cylinder"
- l'ajout de nouveau type de géométrie implicite 3D : ellipsoid, capsule, cone, polyhedra et CSG
- la gestion de géométrie implicite 2D pour la construction CSG : segment, bezier curve, disk, ellipse, solid angle, rectangle, regular polygon, polygon
- la gestion des transformation de base pour tout type de géométrie soit : translation, rotation, scaling

Des modifications au niveau des objets peuplant la scène sont à prévoir :
- l'ajout des nouveaux type de caméra : panoramique, fisheye
- l'ajout d'un profile de lumière pour la ponctual light (Goniophotometric Diagram Map / IES profile)
- (le refactor la sphere light pour s'ajouter à la point light)
- l'ajout d'un nouveau type de volumic light : cylinder
- le refactor des light intensity pour correspondre à des watts ou des lumens
- la gestion de l'instancing
- la création et gestion d'un graph de scène
- l'ajout d'objets de type speakers (pour une potentiel future prise en compte du son)
- l'ajout d'objets de type force field (pour une potentiel simulation du vent)
- la prise en compte des bones and rigging des objets (potentiel prise en compte des mouvements)
- la prise en compte des animations des objets (avec un chrono fonctionel, pour un potentiel prise en comte des animations)

Des modifictions au niveau du materiaux de l'objets sont à venir:
- Ajout d'une couche de clearCoat / Coat
- Ajout d'anisotropie pour le lobe spéculaire
- Ajout d'un lobe "sheen" de rectro réfléction, permet le modélisation de vetements/tissu
- implémentation d'une méthode de compensation d'énergie ou de multiscatering pour GGX (actuelement pas energie conservatif)
- la gestion de la translucence
- la gestion du phénomène d'iridecense
- la simulation de la sub-surface-scatering
- la gestion de texture pour tout les type de paramètre géré par le materiaux
- prise en compte des millieux participants

Autres :
- l'ajout de support pour les environement map
- l'ajout d'un type de json descriptif de scene
- ajout de bidirectional path tracing
- l'ajout d'une interface
- portage sur GPU et utilisation d'embree pour la partie CPU ?
- l'ajout d'une interface graphique
- fusion avec les moteurs RT_ISICG, RT_SDF et RT_Lite
- l'ajout d'effets de post-processing (AgX, denoiser)

# Bugs

- directional light => eclipse en plein millieu
- disk light stratified sampling sous optimal
- alpha en NEE n'est pas prise en compte par l'ombrage => BVH should do other stuff
- Pas de caustics par NEE !
- Le NEE causes pas mal de fireFlyes