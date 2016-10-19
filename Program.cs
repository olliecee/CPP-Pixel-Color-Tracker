using System;
using System.Diagnostics;
using System.Threading;

namespace vamemory_test
{
    class Program
    {
        public static int oLocalPlayer;
        public static int oEntityList;
        public static int oCrossHairID;
        public static int oTeam;
        public static int oHealth;
        public static int oAttack;
        public static int oEntityLoopDistance;

        public static string process = "";
        public static int bClient;

        static void Main(string[] args)
        {
            VAMemory vam = new VAMemory(process);

            if (GetModuleAddy())
            {
                int fAttack = bClient + oAttack;

                while (true)
                {
                    Console.Clear();
                    Console.Write("Not Hitting Target", Console.ForegroundColor = ConsoleColor.Red);

                    int address = bClient + oLocalPlayer;
                    int localplayer = vam.ReadInt32((IntPtr)address);

                    address = localplayer + oTeam;
                    int MyTeam = vam.ReadInt32((IntPtr)address);

                    address = localplayer + oCrossHairID;
                    int PlayerInCross = vam.ReadInt32((IntPtr)address);

                    if (PlayerInCross > 0 && PlayerInCross < 65)
                    {
                        address = bClient + oEntityList + (PlayerInCross - 1) * oEntityLoopDistance;
                        int PtrToPIC = vam.ReadInt32((IntPtr)address);

                        address = PtrToPIC + oHealth;
                        int PICHealth = vam.ReadInt32((IntPtr)address);

                        address = PtrToPIC + oTeam;
                        int PICTeam = vam.ReadInt32((IntPtr)address);

                        if ((PICTeam != MyTeam) && (PICTeam > 1) && (PICHealth > 0))
                        {
                            Console.Clear();
                            Console.Write("Hitting Target", Console.ForegroundColor = ConsoleColor.Green);
                            vam.WriteInt32((IntPtr)fAttack, 1);
                            Thread.Sleep(1);
                            vam.WriteInt32((IntPtr)fAttack, 4);
                        }
                    }
                    Thread.Sleep(50);
                }
            }
        }

        static bool GetModuleAddy()
        {
            try
            {
                Process[] p = Process.GetProcessesByName(process);

                if (p.Length > 0)
                {
                    foreach (ProcessModule m in p[0].Modules)
                    {
                        if (m.ModuleName == "something.dll")
                        {
                            bClient = (int)m.BaseAddress;
                            return true;
                        }
                    }
                    return true;
                }
                else
                {
                    return false;
                }
            }
            catch (Exception ex)
            {
                return false;
            }
        }
    }
}