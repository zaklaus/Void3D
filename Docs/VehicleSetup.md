# Vehicle setup

Setting up a vehicle can be a daunting task in Void3D, given the complexity vehicle handling presents, there are many factors that come into play for a realistic behavior.

NOTE: This guide is specific to Void3D and many steps don't apply to Insanity3D anymore!

## Mission model setup

Mission models, also called prefabs, are single-model missions that can describe the physics behavior as well as actor data within a scene. These can then be instantiated in actual missions.

Hit Alt+N and a prompt for mission name appears, type in `models\vehicles\<car_name>` such as **models\vehicles\niva**.

Hit N to show object lister, select all objects and delete them, our scene has to remain empty. Save now.

Tip: Enable **Helping Light** (F10) to see your model in the scene.

## Modeling process

Model your bodywork and wheels in 3DS MAX and export bodywork to `models\vehicles\<car_name>\scene.i3d`, make sure your wheels are exported as a separate model we will refer to later.

## Dummy setup

Before we set up physics constraints, we need to provide dummies that would be used for that.

### Engine and light brakes

Hit Shift+D and create an `engine` dummy, place it approximately to the front of the vehicle (where an engine would reside).

Hit Shift+D again and create a `light_brake` dummy, place it at where light brakes are (WIP)

### Wheel joints

Now the fun part :)

Create a dummy for each wheel socket and name it `wheel_<left/right>_<front/back>` such as **wheel_l_f**

For forward drive make sure to append `_power` postfix to front wheels. Ex. **wheel_r_f_power**.

Make sure all wheel dummies are rotated 90 degrees to the right.

### Wheels

Hit Shift+M to add a model, select your wheel, link it (B) to the wheel joint and Reset Transform (in 3D object pane), rotate wheel 90 degrees to the left so that it faces forward.

Keep your wheel selected and create a sphere collision volume (Shift+V), scale it to fit your wheel.

Repeat for all wheels.

### Bodywork collision

Add a box collision that would cover an area right above wheels, make sure it's NOT LINKED TO the bodywork visual, but to the Primary Sector!

## Physics setup :^)

You've made it, physics setup awaits now!

Enable Physics Studio first.

Select primary sector, in physics studio pane enable Body and set weight to ex. `1100`, mark it as **Static (debug)**

![test_Jr6NvdBE53](https://user-images.githubusercontent.com/9026786/165064325-f0e8924d-3d9f-4f9b-8925-8b93ec4c67c8.png)

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

## Car exit dummies

By default, player controller tries to approximate to which side they can exit the car, to improve the calculations however, it's recommended to set up exit dummies, called `exit_l`, `exit_r` and `exit_t` for left, right and top exit respectively.

## End result scene tree

Here's how your scene should look like approximately :^)

![test_u65iTYK13J](https://user-images.githubusercontent.com/9026786/165064879-3f47213c-9b0b-474e-9dd2-340c3f011d1b.png)
