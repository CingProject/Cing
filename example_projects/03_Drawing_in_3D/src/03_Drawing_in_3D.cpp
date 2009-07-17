#include "Cing.h"
CREATE_APPLICATION( "Cing Demo" );

/*   
 * This example shows of the creation of 3dObjects and 
 * how to control theirs position and other attributes
 */

std::vector< Sphere >  spheres;          // Aqu� almacenamos los object3D
int                    nBoxes    = 220;  // N�mero de modelos
int                    diameter  = 12;   // Radio de sphere
float                  maxDist   = 160;  // M�x distancia a la que se dibujan uniones
float                  ranValue  = 3;    // Factor de aleatoriedad de movimiento
PointLight             light;			 // Lights in the scene

void setup() {

	size(1024, 768);
	background(94);

	// Modificamos el color y la posicion de la luz
	light.init(0,255,255,0,0,-200);

	// Inicializamos los modelos
	for (int i=0; i< nBoxes; i++) {
		
		// Creamos un nuevo Object3d de tipo Sphere
		Sphere newSphere;

		// Inicializamos su tama�o y posici�n
		newSphere.init(diameter);
		newSphere.setPosition(random(100,width-100),random(100,height-100),0);

		// Por �ltimo la almacenamos dentro del contenedor
		spheres.push_back(newSphere);
	}
}

void draw() {

	// Ahora actualizamos las posiciones de los modelos
	for (int i=0; i< nBoxes; i++) {

		// Accedemos al modelo
		Sphere theSphere = spheres.at(i);

		// modificamos su posicion aleatoriamente
		Vector thePosition = theSphere.getPosition();
		thePosition.x += random(-ranValue,ranValue);
		thePosition.y += random(-ranValue,ranValue);
		thePosition.z += random(-ranValue,ranValue);

		// Toroidal space
		if ( thePosition.x < 0 )      thePosition.x = width;
		if ( thePosition.x > width )  thePosition.x = 0;
		if ( thePosition.y < 0 )      thePosition.y = height;
		if ( thePosition.y > height ) thePosition.y = 0;
		if ( thePosition.z < -300 )   thePosition.z = 300;
		if ( thePosition.z > 300 )    thePosition.z = -300;

		// Actualiza la posici�n del Object3D
		theSphere.setPosition(thePosition);

		// Ahora vamos a ver c�mo usar el comando line para
		// dibujar l�neas en el espacio 3D. Para ello 
		// dibujaremos las uniones entre esta esfera y
		// las m�s cercanas
	    for (int j=0; j< nBoxes; j++) {
			// descartar uniones entre s� mismas
			if (i!=j) {
				Vector theOtherPosition = spheres.at(j).getPosition();
				// Calculamos la distancia
				float dist = thePosition.distance(theOtherPosition);
				// Dibujamos uni�n si la distancia es menor de maxDist
				if ( dist < maxDist ) {
					stroke(255-dist,255-dist);
					line( thePosition.x, thePosition.y, thePosition.z, 
                          theOtherPosition.x, theOtherPosition.y, theOtherPosition.z);
				}
			}	
		}
	}

}

void end()
{
}

void mousePressed()
{
}

void mouseMoved()
{
}

void mouseReleased()
{
}

void keyPressed()
{
}