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

Mirror::Mirror(const glm::vec3& color, float f) : albedo(color), fuzz(f < 1 ? f : 1) {}
bool Mirror::scatter(const ray& rIn, const HitRecord& rec, glm::vec3& attenuation, ray& scattered) const {
	glm::vec3 reflected = reflect(glm::normalize(rIn.getDirection()), rec.normal);
	scattered = ray(rec.hitPoint, reflected + fuzz*randomUnitVector());
	attenuation = albedo;
	return true;
}

//Dielectric Material
Dielectric::Dielectric(float indexOfRefraction) : indexOfRefraction(indexOfRefraction) {}
bool Dielectric::scatter(const ray& rIn, const HitRecord& rec, glm::vec3& attenuation, ray& scattered) const {
	attenuation = glm::vec3(1.0, 1.0, 1.0);
	float refractionRatio = rec.frontFace ? (1.0 / indexOfRefraction) : indexOfRefraction;
	float cosTheta = fmin(dot(-glm::normalize(rIn.getDirection()), rec.normal), 1.0);
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	bool cannotRefract = refractionRatio * sinTheta > 1.0;
	glm::vec3 direction;
	if (cannotRefract || fresnel(cosTheta, refractionRatio) > randomFloat()) {
		direction = reflect(glm::normalize(rIn.getDirection()), rec.normal);
	}
	else {
		direction = refract(glm::normalize(rIn.getDirection()), rec.normal, refractionRatio);
	}
	scattered = ray(rec.hitPoint, direction);
	return true;
}

float Dielectric::fresnel(float cos, float refractionIndex) {
		float f0 = (1 - refractionIndex) / (1 + refractionIndex);
		f0 = f0 * f0;
		return f0 + (1 - f0) * pow((1 - cos), 5);
}

