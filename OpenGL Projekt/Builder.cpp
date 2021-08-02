#include "Builder.h"
#include "Block.h"
#include "Mesh.h"

IndiceType indices[] {
    2, 3, 0,
    0, 1, 2
};

IndiceType indicesReversed[] {
	0, 3, 2,
	2, 1, 0
};

void addFace(BlockFace face, TextureRegion& tr, Mesh& mesh, Vector3G& pos, LightLevel light) {
	Vector3G n = getNormal(face);

	float x = pos.x;
	float y = pos.y;
	float z = pos.z;

	Vector3G color = light.getColor();
	float intensity = light.getIntensity();

	switch(face) {
	case BlockFace::Front: {
		float vertices[] {
			x + 0, y + 0, z + 1, VecCS(n), VecCS(color), intensity, tr.u, tr.v,
			x + 1, y + 0, z + 1, VecCS(n), VecCS(color), intensity, tr.u2, tr.v,
			x + 1, y + 1, z + 1, VecCS(n), VecCS(color), intensity, tr.u2, tr.v2,
			x + 0, y + 1, z + 1, VecCS(n), VecCS(color), intensity, tr.u, tr.v2,
		};
		mesh.addMesh(vertices, 4, indices, 6);
		break;
	}
	case BlockFace::Back: {
		float vertices[] = {
			x + 1, y + 1, z + 0, VecCS(n), VecCS(color), intensity, tr.u2, tr.v2,
			x + 1, y + 0, z + 0, VecCS(n), VecCS(color), intensity, tr.u2, tr.v,
			x + 0, y + 0, z + 0, VecCS(n), VecCS(color), intensity, tr.u, tr.v,
			x + 0, y + 1, z + 0, VecCS(n), VecCS(color), intensity, tr.u, tr.v2,
		};
		mesh.addMesh(vertices, 4, indices, 6);
		break;
	}
	case BlockFace::Right: {
		float vertices[] {
			x + 1, y + 0, z + 1, VecCS(n), VecCS(color), intensity, tr.u, tr.v,
			x + 1, y + 0, z + 0, VecCS(n), VecCS(color), intensity, tr.u2, tr.v,
			x + 1, y + 1, z + 0, VecCS(n), VecCS(color), intensity, tr.u2, tr.v2,
			x + 1, y + 1, z + 1, VecCS(n), VecCS(color), intensity, tr.u, tr.v2,
		};
		mesh.addMesh(vertices, 4, indices, 6);
		break;
	}
	case BlockFace::Left: {
		float vertices[] {
			x + 0, y + 1, z + 0, VecCS(n), VecCS(color), intensity, tr.u2, tr.v2,
			x + 0, y + 0, z + 0, VecCS(n), VecCS(color), intensity, tr.u2, tr.v,
			x + 0, y + 0, z + 1, VecCS(n), VecCS(color), intensity, tr.u, tr.v,
			x + 0, y + 1, z + 1, VecCS(n), VecCS(color), intensity, tr.u, tr.v2,
		};
		mesh.addMesh(vertices, 4, indices, 6);
		break;
	}
	case BlockFace::Top: {
		float vertices[] {
			x + 1, y + 1, z + 1, VecCS(n), VecCS(color), intensity, tr.u2, tr.v2,
			x + 1, y + 1, z + 0, VecCS(n), VecCS(color), intensity, tr.u2, tr.v,
			x + 0, y + 1, z + 0, VecCS(n), VecCS(color), intensity, tr.u, tr.v,
			x + 0, y + 1, z + 1, VecCS(n), VecCS(color), intensity, tr.u, tr.v2,
		};
		mesh.addMesh(vertices, 4, indices, 6);
		break;
	}
	case BlockFace::Bottom: {
		float vertices[] {
			x + 0, y + 0, z + 0, VecCS(n), VecCS(color), intensity, tr.u, tr.v,
			x + 1, y + 0, z + 0, VecCS(n), VecCS(color), intensity, tr.u2, tr.v,
			x + 1, y + 0, z + 1, VecCS(n), VecCS(color), intensity, tr.u2, tr.v2,
			x + 0, y + 0, z + 1, VecCS(n), VecCS(color), intensity, tr.u, tr.v2,
		};
		mesh.addMesh(vertices, 4, indices, 6);
		break;
	}

	}
}

void addXShape(TextureRegion& tr, Mesh& mesh, Vector3G& pos, LightLevel light) {

	float x = pos.x;
	float y = pos.y;
	float z = pos.z;

	Vector3G color = light.getColor();

	float intensity = light.getIntensity();

#define BrokenNormals 1, 1, 1

	float vertices[] {
		x + 0, y + 0, z + 0, BrokenNormals, VecCS(color), intensity, tr.u, tr.v,
		x + 1, y + 0, z + 1, BrokenNormals, VecCS(color), intensity, tr.u2, tr.v,
		x + 1, y + 1, z + 1, BrokenNormals, VecCS(color), intensity, tr.u2, tr.v2,
		x + 0, y + 1, z + 0, BrokenNormals, VecCS(color), intensity, tr.u, tr.v2,
	};
	mesh.addMesh(vertices, 4, indices, 6);
	mesh.addMesh(vertices, 4, indicesReversed, 6);

	float vertices2[] {
		x + 1, y + 0, z + 0, BrokenNormals, VecCS(color), intensity, tr.u, tr.v,
		x + 0, y + 0, z + 1, BrokenNormals, VecCS(color), intensity, tr.u2, tr.v,
		x + 0, y + 1, z + 1, BrokenNormals, VecCS(color), intensity, tr.u2, tr.v2,
		x + 1, y + 1, z + 0, BrokenNormals, VecCS(color), intensity, tr.u, tr.v2,
	};

	mesh.addMesh(vertices2, 4, indices, 6);
	mesh.addMesh(vertices2, 4, indicesReversed, 6);
}
