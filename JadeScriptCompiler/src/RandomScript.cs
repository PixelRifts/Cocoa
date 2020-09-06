

namespace JadeScriptRuntime
{
	class RandomComponent : ScriptableComponent
	{
		public override void Start()
		{
			Debug.LogInfo("Now I'm changing the value at runtime.... OMGGGGG");
		}

		override public void Update(float dt)
		{
			Debug.LogInfo($"Updating Random Component, dt: {dt}");
		}
	}
}
