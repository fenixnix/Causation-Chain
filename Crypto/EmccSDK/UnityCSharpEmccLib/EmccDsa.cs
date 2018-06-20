using System;
using System.Runtime.InteropServices;
using System.Text;
using UnityEngine;
using System.Security.Cryptography;

public class EmccDsa : MonoBehaviour {

    [DllImport("EmccLib")]
    private static extern int GetInfo(StringBuilder data);

    [DllImport("EmccLib")]
    private static extern void CreateKey();

    [DllImport("EmccLib")]
    private static extern int GetPrivateKey(StringBuilder data);
    [DllImport("EmccLib")]
    private static extern int GetPublicKey(StringBuilder data);
    
    [DllImport("EmccLib")]
    private static extern int Sign(string priKey, string msg);

    [DllImport("EmccLib")]
    private static extern int GetSign(StringBuilder sign);

    [DllImport("EmccLib")]
    private static extern bool Verify(string pubKey, string msg, string sign);

    public string PrivateKey()
    {
        StringBuilder data = new StringBuilder(192);
        int l = GetPrivateKey(data);
        Debug.Log("PrivateKey Size:" + l);
        return data.ToString();
    }

    public string PublicKey()
    {
        StringBuilder data = new StringBuilder(128);
        int l = GetPublicKey(data);
        Debug.Log("PublicKey Size:" + l);
        return data.ToString();
    }

    public string SignData()
    {
        StringBuilder data = new StringBuilder(512);
        int l = GetSign(data);
        Debug.Log("Sign Size:" + l);
        return data.ToString();
    }

    public static string SHA256(string str)
    {
        //如果str有中文，不同Encoding的sha是不同的！！
        byte[] SHA256Data = Encoding.UTF8.GetBytes(str);

        SHA256Managed Sha256 = new SHA256Managed();
        byte[] by = Sha256.ComputeHash(SHA256Data);

        return BitConverter.ToString(by).Replace("-", "").ToLower(); //64
                                                                     //return Convert.ToBase64String(by);                         //44
    }

    public string[] GetKeyPair()
    {
        CreateKey();
        string[] pair = { PrivateKey(), PublicKey() };
        Debug.Log("PrivateKey:" + pair[0]);
        Debug.Log("PublicKey:" + pair[1]);
        return pair;
    }

    public string SignMsg(string priKey, string msg)
    {
        string data = SHA256(msg);
        Sign(priKey, data);
        return SignData();
    }

    public bool VerifyMsg(string pubKey, string msg, string sign)
    {
        return Verify(pubKey, SHA256(msg), sign);
    }

    public void SelfTest()
    {
        var keyPair = GetKeyPair();
        var sign = SignMsg(keyPair[0], "Hello");
        var res = VerifyMsg(keyPair[1], "Hello", sign);
        Debug.Log("Sign:" + sign);
        Debug.Log(res);
    }
}
