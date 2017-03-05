


/* This file offers a camera data type that can be used in two ways. The 
low-level interface lets you control the position, orientation, and projection 
of the camera directly --- flexible but inconvenient. The high-level interface 
let you control the camera by aiming it at a target in a certain third-person 
way --- inflexible, but convenient if that's what you want to do. I do not 
recommend mixing the two ways; pick one or the other. */

/* Feel free to read from this struct's members, but don't write to them, 
except through accessor functions. */
typedef struct camCamera camCamera;
struct camCamera {
	/* Low-level interface. */
	GLdouble rotation[3][3];
	GLdouble translation[3];
	GLdouble projection[6];
	GLuint projectionType;
	/* High-level interface. */
	GLdouble fovy, ratio, width, height;
	GLdouble distance;
	GLdouble phi, theta;
	GLdouble target[3];
};



/*** Low-level interface ***/

#define camORTHOGRAPHIC 0
#define camPERSPECTIVE 1
#define camPROJL 0
#define camPROJR 1
#define camPROJB 2
#define camPROJT 3
#define camPROJF 4
#define camPROJN 5

/* Sets the camera's rotation. */
void camSetRotation(camCamera *cam, GLdouble rot[3][3]) {
	vecCopy(9, (GLdouble *)rot, (GLdouble *)(cam->rotation));
}

/* Sets the camera's translation. */
void camSetTranslation(camCamera *cam, GLdouble transl[3]) {
	vecCopy(3, transl, cam->translation);
}

/* Sets the projection type, to either camORTHOGRAPHIC or camPERSPECTIVE. */
void camSetProjectionType(camCamera *cam, GLuint projType) {
	cam->projectionType = projType;
}

/* Sets all six projection parameters. */
void camSetProjection(camCamera *cam, GLdouble proj[6]) {
	vecCopy(6, proj, cam->projection);
}

/* Sets one of the six projection parameters. */
void camSetOneProjection(camCamera *cam, GLuint i, GLdouble value) {
	cam->projection[i] = value;
}

/* Sets the camera's rotation and translation, in a manner suitable for third-
person viewing. The camera is aimed at the world coordinates target. The camera 
itself is displaced from that target by a distance rho, in the direction 
specified by the spherical coordinates phi and theta (as in vec3Spherical). 
Under normal use, where 0 < phi < pi, the camera's up-direction is world-up, 
or as close to it as possible. */
void camLookAt(camCamera *cam, GLdouble target[3], GLdouble rho, GLdouble phi, 
		GLdouble theta) {
	GLdouble z[3], y[3], yStd[3] = {0.0, 1.0, 0.0}, zStd[3] = {0.0, 0.0, 1.0};
	vec3Spherical(1.0, phi, theta, z);
	vec3Spherical(1.0, M_PI / 2.0 - phi, theta + M_PI, y);
	mat33BasisRotation(yStd, zStd, y, z, cam->rotation);
	vecScale(3, rho, z, cam->translation);
	vecAdd(3, target, cam->translation, cam->translation);
}

/* Sets the camera's rotation and translation, in a manner suitable for first-
person viewing. The camera is positioned at the world coordinates position. 
From that position, the camera's sight direction is described by the spherical 
coordinates phi and theta (as in vec3Spherical). Under normal use, where 
0 < phi < pi, the camera's up-direction is world-up, or as close to it as 
possible. */
void camLookFrom(camCamera *cam, GLdouble position[3], GLdouble phi, 
		GLdouble theta) {
	GLdouble negZ[3], y[3];
	GLdouble yStd[3] = {0.0, 1.0, 0.0}, negZStd[3] = {0.0, 0.0, -1.0};
	vec3Spherical(1.0, phi, theta, negZ);
	vec3Spherical(1.0, M_PI / 2.0 - phi, theta + M_PI, y);
	mat33BasisRotation(yStd, negZStd, y, negZ, cam->rotation);
	vecCopy(3, position, cam->translation);
}

/* Sets the projection type and the six projection parameters, based on the 
width and height of the viewport and three other parameters. The camera looks down the center of the viewing volume. For perspective projection, fovy is the 
full (not half) vertical angle of the field of vision, in radians. focal > 0 is 
the distance from the camera to the 'focal' plane (where 'focus' is used in the 
sense of attention, not optics). ratio expresses the far and near clipping 
planes relative to focal: far = -focal * ratio and near = -focal / ratio. 
Reasonable values are fovy = M_PI / 6.0, focal = 10.0, and ratio = 10.0, so 
that far = -100.0 and near = -1.0. For orthographic projection, the projection 
parameters are set to produce the orthographic projection that, at the focal 
plane, is most similar to the perspective projection just described. */
void camSetFrustum(camCamera *cam, GLuint projType, GLdouble fovy, 
		GLdouble focal, GLdouble ratio, GLdouble width, GLdouble height) {
	cam->projectionType = projType;
	cam->projection[camPROJF] = -focal * ratio;
	cam->projection[camPROJN] = -focal / ratio;
	if (projType == camPERSPECTIVE)
		cam->projection[camPROJT] = -cam->projection[camPROJN] 
			* tan(fovy * 0.5);
	else
		cam->projection[camPROJT] = focal * tan(fovy * 0.5);
	cam->projection[camPROJB] = -cam->projection[camPROJT];
	cam->projection[camPROJR] = cam->projection[camPROJT] * width / height;
	cam->projection[camPROJL] = -cam->projection[camPROJR];
}

/* viewingLoc is a shader location for a uniform 4x4 matrix. This function 
loads that location with the camera's inverse isometry and projection --- that 
is, P C^-1, in the notation of our software graphics engine. */
void camRender(camCamera *cam, GLint viewingLoc) {
	GLfloat viewing[4][4];
	double camInv[4][4], proj[4][4], projCamInv[4][4];
	mat44InverseIsometry(cam->rotation, cam->translation, camInv);

	//call proj matrix builder based on projectionType
	if (cam->projectionType == camPERSPECTIVE)
	    mat44Perspective(cam->projection[camPROJL], cam->projection[camPROJR], 
	        cam->projection[camPROJB], cam->projection[camPROJT], cam->projection[camPROJF], 
	        cam->projection[camPROJN], proj);
	else
	    mat44Orthographic(cam->projection[camPROJL], cam->projection[camPROJR], 
	        cam->projection[camPROJB], cam->projection[camPROJT], cam->projection[camPROJF], 
	        cam->projection[camPROJN], proj);
	mat444Multiply(proj, camInv, projCamInv);
	mat44OpenGL(projCamInv, viewing);
	glUniformMatrix4fv(viewingLoc, 1, GL_FALSE, (GLfloat *)viewing);
}


/*** High-level interface ***/

void camSetControls(camCamera *cam, GLuint projType, GLdouble fovy, 
		GLdouble ratio, GLdouble width, GLdouble height, GLdouble distance, 
		GLdouble phi, GLdouble theta, GLdouble target[3]) {
	cam->fovy = fovy;
	cam->ratio = ratio;
	cam->width = width;
	cam->height = height;
	cam->distance = distance;
	cam->phi = phi;
	cam->theta = theta;
	vecCopy(3, target, cam->target);
	camSetProjectionType(cam, projType);
	camSetFrustum(cam, cam->projectionType, cam->fovy, cam->distance, 
		cam->ratio, cam->width, cam->height);
	camLookAt(cam, cam->target, cam->distance, cam->phi, cam->theta);
}

void camSwitchProjectionType(camCamera *cam) {
	if (cam->projectionType == camPERSPECTIVE)
		camSetProjectionType(cam, camORTHOGRAPHIC);
	else
		camSetProjectionType(cam, camPERSPECTIVE);
	camSetFrustum(cam, cam->projectionType, cam->fovy, cam->distance, 
		cam->ratio, cam->width, cam->height);
}

void camAddFovy(camCamera *cam, GLdouble delta) {
	cam->fovy += delta;
	camSetFrustum(cam, cam->projectionType, cam->fovy, cam->distance, 
		cam->ratio, cam->width, cam->height);
}

void camAddRatio(camCamera *cam, GLdouble delta) {
	cam->ratio += delta;
	camSetFrustum(cam, cam->projectionType, cam->fovy, cam->distance, 
		cam->ratio, cam->width, cam->height);
}

void camSetWidthHeight(camCamera *cam, GLdouble width, GLdouble height) {
	cam->width = width;
	cam->height = height;
	camSetFrustum(cam, cam->projectionType, cam->fovy, cam->distance, 
		cam->ratio, cam->width, cam->height);
}

void camAddDistance(camCamera *cam, GLdouble delta) {
	cam->distance += delta;
	camSetFrustum(cam, cam->projectionType, cam->fovy, cam->distance, 
		cam->ratio, cam->width, cam->height);
	camLookAt(cam, cam->target, cam->distance, cam->phi, cam->theta);
}

void camAddPhi(camCamera *cam, GLdouble delta) {
	cam->phi += delta;
	camLookAt(cam, cam->target, cam->distance, cam->phi, cam->theta);
}

void camAddTheta(camCamera *cam, GLdouble delta) {
	cam->theta += delta;
	camLookAt(cam, cam->target, cam->distance, cam->phi, cam->theta);
}

void camSetTarget(camCamera *cam, GLdouble target[3]) {
	vecCopy(3, target, cam->target);
	camLookAt(cam, cam->target, cam->distance, cam->phi, cam->theta);
}


