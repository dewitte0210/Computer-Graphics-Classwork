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
	Mirror(const glm::vec3& a);
	bool scatter(const ray& rIn, const HitRecord& rec, glm::vec3& attenuation, ray& scattered) const;
private:
	glm::vec3 albedo;
};
