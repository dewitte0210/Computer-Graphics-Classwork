#pragma once
#include "mathFunctions.h"
#include "hittable.h"

class Material {
public:
	virtual ~Material() = default;
	virtual bool scatter(const ray& rIn, const HitRecord& rec, glm::vec3& attenuation, ray& scattered) const = 0;
};

class Lambertian : public Material {
public :
	Lambertian(const glm::vec3& color);
	bool scatter(const ray& rIn, const HitRecord& rec, glm::vec3& attenuation, ray& scattered) const;
private:
	glm::vec3 albedo;
};

class Mirror : public Material {
public:
	Mirror(const glm::vec3& a, float f);
	bool scatter(const ray& rIn, const HitRecord& rec, glm::vec3& attenuation, ray& scattered) const;
private:
	glm::vec3 albedo;
	float fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(float indexOfRefraction);
	bool scatter(const ray& rIn, const HitRecord& rec, glm::vec3& attenuation, ray& scattered) const;
private:
	float indexOfRefraction;
	static float fresnel(float cos, float refractionIndex);
};
