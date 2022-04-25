# Vehicle setup

Setting up a vehicle can be a daunting task in Void3D, given the complexity vehicle handling presents, there are many factors that come into play for a realistic behavior.

NOTE: This guide is specific to Void3D and many steps don't apply to Insanity3D anymore!

## Mission model setup

Mission models, also called prefabs, are single-model missions that can describe the physics behavior as well as actor data within a scene. These can then be instantiated in actual missions.

Hit Alt+N and a prompt for mission name appears, type in `models\vehicles\<car_name>` such as **models\vehicles\niva**.
![test_sCQJWqchaG](https://user-images.githubusercontent.com/9026786/165065737-c51ab8e0-8455-488e-a53a-ac38f8466ebd.png)

Hit N to show object lister, select all objects and delete them, our scene has to remain empty. Save now.
![test_AbEskkDsWj](https://user-images.githubusercontent.com/9026786/165065828-45484483-3700-4e22-87a4-d5aac59ffd2f.png)

Tip: Enable **Helping Light** (F10) in the Debug toolbar to see your model in the scene.
![3dsmax_-_Copy_VENrhJLXTI](https://user-images.githubusercontent.com/9026786/165065949-2c933bf8-9d9d-45b8-b90b-c9fb86160d92.png)

## Modeling process

Model your bodywork and wheels in 3DS MAX and export bodywork to `models\vehicles\<car_name>\scene.i3d`, make sure your wheels are exported as a separate model we will refer to later.
![3dsmax_-_Copy_makE2vBs8F](https://user-images.githubusercontent.com/9026786/165065588-424bb7a0-6f31-46bc-a73c-2229bec0800b.png)

## Dummy setup

Before we set up physics constraints, we need to provide dummies that would be used for that.

### Engine and light brakes

Hit Shift+D and create an `engine` dummy, place it approximately to the front of the vehicle (where an engine would reside).

Hit Shift+D again and create a `light_brake` dummy, place it at where light brakes are (WIP)

### Wheel joints

Now the fun part :)

Create a dummy for each wheel socket and name it `wheel_<left/right>_<front/back>` such as **wheel_l_f**

For forward drive make sure to append `_power` postfix to front wheels. Ex. **wheel_r_f_power**.
![test_r60pg4wAOT](https://user-images.githubusercontent.com/9026786/165066132-6bfda7b2-baff-4027-a7db-609b5e93a620.png)

Make sure all wheel dummies are rotated 90 degrees to the right.

### Wheels

Hit Shift+M to add a model, select your wheel, link it (B) to the wheel joint and Reset Transform (in 3D object pane), rotate wheel 90 degrees to the left so that it faces forward.

Keep your wheel selected and create a sphere collision volume (Shift+V), scale it to fit your wheel.

![test_xPh3KywKlt](https://user-images.githubusercontent.com/9026786/165065299-a1dce0bc-b7bb-4745-bf32-0f5ff9c48943.png)

Repeat for all wheels.

### Bodywork collision

Add a box collision that would cover an area right above wheels, make sure it's NOT LINKED TO the bodywork visual, but to the Primary Sector!

![test_Jr6NvdBE53](https://user-images.githubusercontent.com/9026786/165064325-f0e8924d-3d9f-4f9b-8925-8b93ec4c67c8.png)

## Physics setup :^)

You've made it, physics setup awaits now!

Enable Physics Studio first.

Select primary sector, in physics studio pane enable Body and set weight to ex. `1100`, mark it as **Static (debug)**

Per each wheel (model), enable Body and set weight to ex. `400`

Per each wheel front (driving) joint, enable Joint and set up the following parameters:
```
Type: Hinge2
Lo 0: -20   Hi 0: 20
Max force: 0.3
Fudge: 0.2

Susp.ERP: 0.5
Susp.CFM: 0.002
```
For all other wheel joints, enable Joint and set up the following parameters:
```
Type: Hinge2
Lo 0: 0   Hi 0: 0
Max force: 0.03

Susp.ERP: 0.5
Susp.CFM: 0.002
```

![test_i2xPFnJkew](https://user-images.githubusercontent.com/9026786/165064709-039621d1-7a34-4f2e-8b2e-be25ad319212.png)

## Engine setup

Select bodywork and hit A, select Vehicle actor and confirm.

Hit A again, a table shows up, here we configure engine gears, max. camera distance and other goodies. Feel free to Import existing config from **Tables\Vehicles** to understand how to set the values up.
![test_4BgSpmtrp4](https://user-images.githubusercontent.com/9026786/165066709-9ed38b74-360e-41a0-a44f-b16fd7474f8a.png)

## Car exit dummies

By default, player controller tries to approximate to which side they can exit the car, to improve the calculations however, it's recommended to set up exit dummies, called `exit_l`, `exit_r` and `exit_t` for left, right and top exit respectively.
![test_xXy977Uye7](https://user-images.githubusercontent.com/9026786/165066607-8d2b61d5-6eaf-4ac1-943a-adacf3a62413.png)

## End result scene tree

Here's how your scene should look like approximately :^)

![test_u65iTYK13J](https://user-images.githubusercontent.com/9026786/165064879-3f47213c-9b0b-474e-9dd2-340c3f011d1b.png)
