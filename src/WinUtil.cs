using System;
using System.Runtime.InteropServices;

namespace TTT5
{
    /// <summary>
    /// Console utilities exclusive to Windows systems.
    /// </summary>
    public static class WinUtil
    {
        private const int StdInputHandle = -10;
        private const int StdOutputHandle = -11;
        private const uint EnableMouseInput = 0x0010;
        private const uint EnableQuickEdit = 0x0040;

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool GetConsoleMode(IntPtr consoleHandle, out uint mode);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool SetConsoleMode(IntPtr consoleHandle, uint mode);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr GetStdHandle(int stdHandle);


        /// <summary>Disables mouse input and selection in the console on Windows if possible.</summary>
        public static void DisableMouse()
        {
            try
            {
                var console = GetStdHandle(StdInputHandle);
                if (!GetConsoleMode(console, out var mode)) return;
                SetConsoleMode(console, mode & ~EnableMouseInput & ~EnableQuickEdit);
            }
            catch { } // Not important
        }
    }
}
