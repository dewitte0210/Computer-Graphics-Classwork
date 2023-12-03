#include "materials.h"

// File holds definitions for all material classes

// Lambertian Material
Lambertian::Lambertian(const glm::vec3& color) : albedo(color) {}

bool Lambertian::scatter(const ray& rIn, const HitRecord& rec, glm::vec3& attenuation, ray& scattered) const {
	glm::vec3 scatterDirection = rec.normal + randomUnitVector();
	
	//Catches a bad reflection case
	if (nearZero(scatterDirection)) {
		scatterDirection = rec.normal;
	}
	scattered = ray(rec.hitPoint, scatterDirection);
	attenuation = albedo;
	return true;
}

//Mirror Material

Mirror::Mirror(const glm::vec3& color) : albedo(color) {}
bool Mirror::scatter(const ray& rIn, const HitRecord& rec, glm::vec3& attenuation, ray& scattered) const {
	glm::vec3 reflected = reflect(glm::normalize(rIn.getDirection()), rec.normal);
	scattered = ray(rec.hitPoint, reflected);
	attenuation = albedo;
	return true;
}
