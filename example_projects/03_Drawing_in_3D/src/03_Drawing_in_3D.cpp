#include "Cing.h"
CREATE_APPLICATION( "Cing Demo" );

/*   
 * This example shows of the creation of 3dObjects and 
 * how to control theirs position and other attributes
 */

std::vector< Sphere >  spheres;          // Aquí almacenamos los object3D
int                    nBoxes    = 220;  // Número de modelos
int                    diameter  = 12;   // Radio de sphere
float                  maxDist   = 160;  // Máx distancia a la que se dibujan uniones
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

		// Inicializamos su tamaño y posición
		newSphere.init(diameter);
		newSphere.setPosition(random(100,width-100),random(100,height-100),0);

		// Por último la almacenamos dentro del contenedor
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

		// Actualiza la posición del Object3D
		theSphere.setPosition(thePosition);

		// Ahora vamos a ver cómo usar el comando line para
		// dibujar líneas en el espacio 3D. Para ello 
		// dibujaremos las uniones entre esta esfera y
		// las más cercanas
	    for (int j=0; j< nBoxes; j++) {
			// descartar uniones entre sí mismas
			if (i!=j) {
				Vector theOtherPosition = spheres.at(j).getPosition();
				// Calculamos la distancia
				float dist = thePosition.distance(theOtherPosition);
				// Dibujamos unión si la distancia es menor de maxDist
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