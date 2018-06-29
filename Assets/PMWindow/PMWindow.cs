using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;


public class PMWindowAPI
{
    // public api
    [DllImport("PMWindow")]
    public static extern void SetWindowVisible(bool visible);

    // private api

    [DllImport("PMWindow")]
    private static extern void set_log_function_wide(System.IntPtr logFunction);

    [DllImport("PMWindow")]
    private static extern void resetImGui();

    [DllImport("PMWindow")]
    private static extern void beginImGuiDrawing();

    [DllImport("PMWindow")]
    private static extern void endImGuiDrawing();

    [DllImport("PMWindow")]
    private static extern void beginWindow(string label, float x, float y, float w, float h);

    [DllImport("PMWindow")]
    private static extern void imguiEnd();


    [StructLayout(LayoutKind.Sequential)]
    struct FloatParam
    {
        [MarshalAs(UnmanagedType.I1)]
        public bool touch;

        public float value;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct BoolParam
    {
        [MarshalAs(UnmanagedType.I1)]
        public bool touch;

        [MarshalAs(UnmanagedType.I1)]
        public bool value;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct IntParam
    {
        [MarshalAs(UnmanagedType.I1)]
        public bool touch;

        public int value;
    }

    [DllImport("PMWindow")]
    private static extern bool treeNode(string label, bool defaultOpen);

    [DllImport("PMWindow")]
    private static extern void treePop();

    [DllImport("PMWindow")]
    private static extern void inputFloat(string label, ref FloatParam param, float step);

    [DllImport("PMWindow")]
    private static extern void sliderFloat(string label, ref FloatParam param, float minValue, float maxValue);

    [DllImport("PMWindow")]
    private static extern void checkbox(string label, ref BoolParam param);

    [DllImport("PMWindow")]
    private static extern void inputInt(string label, ref IntParam param);


    [DllImport("PMWindow")]
    private static extern void text(string label);

    public static void ResetImGui()
    {
        if (insideImGui)
        {
            Debug.LogError("ImGui can't call inside ImGui");
            return;
        }
        resetImGui();
    }

    private static bool insideImGui = false;

    public static bool TreeNode(string label, bool defaultOpen)
    {
        return treeNode(label, defaultOpen);
    }
    public static void TreePop()
    {
        treePop();
    }

    public static bool InputFloat(string label, ref float value, float step)
    {
        FloatParam param;
        param.value = value;
        param.touch = false;
        inputFloat(label, ref param, step);

        value = param.value;

        return param.touch;
    }
    public static bool SliderFloat(string label, ref float value, float minValue, float maxValue)
    {
        FloatParam param;
        param.value = value;
        param.touch = false;
        sliderFloat(label, ref param, minValue, maxValue);

        value = param.value;

        return param.touch;
    }
    public static bool InputInt(string label, ref int value)
    {
        IntParam param;
        param.value = value;
        param.touch = false;
        inputInt(label, ref param);

        value = param.value;

        return param.touch;
    }
    public static bool Checkbox(string label, ref bool value)
    {
        BoolParam param;
        param.value = value;
        param.touch = false;
        checkbox(label, ref param);

        value = param.value;

        return param.touch;
    }

    public static void Text(string label)
    {
        text(label);
    }
    public static void Text(string format, params object[] values)
    {
        text(string.Format(format, values));
    }

    public static void BeginWindow(string title, float x, float y, float w, float h)
    {
        if (insideImGui == false)
        {
            Debug.LogError("Window must be called inside ImGui");
            return;
        }

        beginWindow(title, x, y, w, h);
    }
    public static void EndWindow()
    {
        imguiEnd();
    }

    public static void BeginImGuiDrawing()
    {
        if(insideImGui)
        {
            Debug.LogError("ImGui can't call inside ImGui");
            return;
        }

        insideImGui = true;
        beginImGuiDrawing();
    }
    public static void EndImGuiDraing()
    {
        if (insideImGui == false)
        {
            Debug.LogError("ImGui can't call inside ImGui");
            return;
        }
        endImGuiDrawing();
        insideImGui = false;
    }


    public static void SetupLogging()
    {
        Action<System.IntPtr> logFunction = (name_ptr) => {
            string text = Marshal.PtrToStringUni(name_ptr);
            Debug.Log(text);
        };
        set_log_function_wide(Marshal.GetFunctionPointerForDelegate(logFunction));
    }
}
