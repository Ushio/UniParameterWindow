using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Controller : MonoBehaviour {
    public float floatValue = 0;
    public bool boolValue = false;
    public int intValue = 0;

	// Use this for initialization
	void Start () {
        PMWindowAPI.ResetImGui();
    }
	
	// Update is called once per frame
	void Update () {
        transform.position = new Vector3(floatValue, 0, 0);

        PMWindowAPI.BeginImGuiDrawing();

        PMWindowAPI.BeginWindow("config", 100, 100, 300, 300);

        PMWindowAPI.Text("Hello World");

        if (PMWindowAPI.TreeNode("parameters", true))
        {
            PMWindowAPI.SliderFloat("slider", ref floatValue, 0.0f, 1.0f);
            PMWindowAPI.InputFloat("inputFloat", ref floatValue, 0.1f);
            PMWindowAPI.Checkbox("checkbox", ref boolValue);
            PMWindowAPI.InputInt("inputInt", ref intValue);

            PMWindowAPI.TreePop();
        }

        if (PMWindowAPI.TreeNode("texts", true))
        {
            PMWindowAPI.Text("floatValue = {0}", floatValue);
            PMWindowAPI.Text("Hey");
            PMWindowAPI.Text("Hello World");
            PMWindowAPI.TreePop();
        }

        PMWindowAPI.EndWindow();

        PMWindowAPI.EndImGuiDraing();
    }
    private void OnEnable()
    {
        PMWindowAPI.SetupLogging();
    }
    private void OnDisable()
    {
    }
}
