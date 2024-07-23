# Solutions to Exercise Session 1

1. Setting up the SMB simulation

Before we begin the SMB simulation, we need to download the set of packages `smb_common`. We can do this by downloading it on the host machine, extracting and copying it into the container.

Once we have downloaded and extracted `smb_common.zip`, we can copy the folder `smb_common` into `custom_ros:/root/git` using the following.

``` bash
docker cp /path/to/smb_common custom_ros:/root/git/.
```

Next we create our workspace inside the container and create a symlink using,

``` bash
cd
mkdir ~/Workspaces/smb_ws/src
ln -s ~/git/ ~/Workspaces/smb_ws/src/
```

Now, we can install the dependencies that `rosdep` finds using,

``` bash
rosdep check --from-paths ~/Workspaces/smb_ws/src --ignore-src
```

Unfortunately, in my case it could not find a package, leading to build errors down the line. I installed it using,

``` bash
sudo apt install ros-noetic-gazebo-plugins
```

Once all the dependencies are installed, we can go ahead and compile the `smb_gazebo` package using,

``` bash
cd ~/Workspaces/smb_ws/
catkin build smb_gazebo
source devel/setup.bash
```

---

2. Launching the simulation

``` bash
roslaunch smb_gazebo smb_gazebo.launch
```

As of now, this does NOT show the GUI, due to some error in libGL. Working on it.
However, the rest of the process behaves as expected, giving us the necessary rostopics & rosnodes.

---

3. Setting a velocity

We can get a list of nodes related to velocity using,

``` bash
rostopic list | grep "vel"
```

Among the ones listed, `cmd_vel` seems to be the relevant one. We can get it's type using,

``` bash
rostopic type /cmd_vel
```

Which tells us that the type is `geometry_msgs/Twist`, the format of which is

```
Vector3 linear
    float64 x
    float64 y
    float64 z

Vector3 angular
    float64 x
    float64 y
    float64 z
```

We can now provide a custom velocity to our robot.

``` bash
rostopic pub /cmd_vel geometry_msgs/Twist "linear:
    x: 1.0
    y: 2.0
    z: 3.0
angular:
    x: 1.0
    y: 2.0
    z: 3.0"
```

Source: https://docs.ros.org/en/diamondback/api/geometry_msgs/html/msg/Twist.html

---

4. Controlling the robot using keyboard

Cloning the repository.

``` bash
cd ~/git
git clone https://github.com/ros-teleop/teleop-twist-keyboard
```

Compiling the package.

``` bash
cd ~/Workspaces/smb_ws/
catkin build teleop_twist_keyboard
source devel/setup.bash
```

Running teleop_twist_keyboard after starting the simulation as shown above.

``` bash
rosrun teleop_twist_keyboard teleop_twist_keyboard.py
```

We can observe the changes to the robot by subscribing to the topic `/cmd_vel`.
So, on another terminal, we can type the following.

``` bash
rostopic echo /cmd_vel
```

Source: https://github.com/ros-teleop/teleop_twist_keyboard

---

5. Writing a custom launch file

The custom launch file should be placed in `~/git/smb_common/smb_gazebo/launch/`.

``` bash
cd ~/git/smb_common/smb_gazebo/launch
touch smb_gazebo_diff.launch
vim smb_gazebo_diff.launch
```

Place the following content inside `smb_gazebo_diff.launch`.

``` xml

<?xml version="1.0" encoding="utf-8"?>

<launch>
  <include file="$(find smb_gazebo)/launch/smb_gazebo.launch">

    <arg name="world_file"	value="worlds/robocup14_spl_field.world"/>

  </include>
</launch>

```

We can now launch `smb_gazebo` with this custom launch file.

``` bash
roslaunch smb_gazebo smb_gazebo_diff.launch
```

Source: https://wiki.ros.org/roslaunch/XML/include
