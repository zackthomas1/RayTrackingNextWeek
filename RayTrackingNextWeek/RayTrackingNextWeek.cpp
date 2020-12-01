// RayTracingOneWeekend.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>

#include "rtweekend.h"
#include "vec3.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "camera.h"
#include "material.h"
#include "aarect.h"
#include "box.h"


// Consider moving in ray header file/ ray class

/// <summary>
/// returns the color value of a ray cast into a scene
/// </summary>
/// <param name="r"></param>
/// <param name="background"></param>
/// <param name="world"></param>
/// <param name="depth"></param>
/// <returns></returns>
color ray_color(const ray& r,const color& background, const hittable& world, int depth) {

	hit_record rec;

	// If we've exceeded the ray bounce limit, no morelight is gathered. 
	// Prevents infinity recursion.
	if (depth <= 0)
		return color(0, 0, 0);

	// If the ray hits nothing, return the background color.
	if (!world.hit(r, 0.001, infinity, rec)) {
		return background;
	}

	ray scattered;
	color attenuation;
	color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

	if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * ray_color(scattered, background, world, depth - 1);

	//// if NO object is hit render default sky background
	//vec3 unit_direction = unit_vector(r.direction());
	//auto t = 0.5 * (unit_direction.y() + 1.0);
	//return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); // linear interpolation
	//// blendValue = (1 -t) * startValue + t * endValue
}

// Scenes
hittable_list random_scene() {

	hittable_list world;

	auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
	auto ground_material = make_shared<lambertian>(checker);
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// Diffuse 
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					auto center2 = center + vec3(0, random_double(0, 0.75), 0);
					world.add(make_shared<moving_sphere>(
						center, center2, 0.0, 1.0, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal 
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass 
					sphere_material = make_shared<dielectric>(1.5);
					double randomRadius = random_double(0.1, 0.9);
					world.add(make_shared<sphere>(center, randomRadius, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, ground_material));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}
hittable_list two_spheres() {
	hittable_list objects; 

	auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
	auto checker_material = make_shared<lambertian>(checker);

	objects.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
	objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

	return objects;
}
hittable_list two_perlin_spheres() {
	hittable_list objects; 

	auto pertext = make_shared<noise_texture>(4); 
	auto pertext_material = make_shared<lambertian>(pertext);

	objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, pertext_material));
	objects.add(make_shared<sphere>(point3(0, 2, 0), 2, pertext_material));

	return objects;
}
//hittable_list earth(){}
hittable_list simple_light() {
	hittable_list objects;

	auto pertext = make_shared<noise_texture>(4);
	auto pertext_material = make_shared<lambertian>(pertext);

	objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, pertext_material));
	objects.add(make_shared<sphere>(point3(0, 2, 0), 2, pertext_material));

	auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
	objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
	objects.add(make_shared<sphere>(point3(0, 8, 0), 3, difflight));

	return objects;
}
hittable_list cornell_box() {
	hittable_list objects;

	auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
	auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
	auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
	auto light = make_shared<diffuse_light>(color(15, 15, 15));

	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<xz_rect>(150, 400, 200, 350, 554, light));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

	shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	objects.add(box1);

	shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
	//box2 = make_shared<rotate_x>(box2, -20);
	box2 = make_shared<rotate_y>(box2, -18);
	//box2 = make_shared<rotate_z>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 1, 65));
	objects.add(box2);

	return objects;
}


int main()
{
	//	Image 
	auto aspect_ratio = 16.0 / 9.0;
	int samples_per_pixel = 100;  // Sets anti-alaising samples
	int image_width = 480;
	int max_depth = 50;	// sets recussive limt for ray_color function

	// World 
	hittable_list world;
	
	point3 lookfrom;
	point3 lookat;
	auto vfov = 40.0;
	auto aperture = 0.0;
	color background(0, 0, 0);

	// Choosen a scene
	switch (6)
	{
	case 1:
		world = random_scene();
		background = color(0.70, 0.80, 1.00);
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		vfov = 20.0;
		aperture = 0.1;
		break;

	case 2:
		world = two_spheres();
		background = color(0.70, 0.80, 1.00);
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		vfov = 20.0;
		break;

	case 3:
		world = two_perlin_spheres(); 
		background = color(0.70, 0.80, 1.00);
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		vfov = 20.0;
		break;

	//case 4:
	//	world = earth();
	//	background = color(0.70, 0.80, 1.00);
	//	lookfrom = point3(13, 2, 3);
	//	lookat = point3(0, 0, 0);
	//	vfov = 20.0;
	//	break;

	case 5:
		world = simple_light();
		samples_per_pixel = 400;
		background = color(0, 0, 0);
		lookfrom = point3(26, 3, 6);
		lookat = point3(0, 2, 0);
		vfov = 20.0;
		break;
	
	case 6:
		world = cornell_box();
		aspect_ratio = 1.0;
		image_width = 600;
		samples_per_pixel = 200;
		background = color(0, 0, 0);
		lookfrom = point3(278, 278, -800);
		lookat = point3(278, 278, 0);
		vfov = 40.0;
		break;

	default:
	case 7:
		background = color(0.0, 0.0, 0.0);
		break;
	}

	// Camera 
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

	//	Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--) // row
	{
		std::cerr << "\rScalinesRemaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i)	// column
		{
			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; s++) { // anti-aliasing subpixel rays 
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, background, world, max_depth);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
	return 0;
}