using System;
using System.Globalization;

namespace TestCasesApp
{
  /// <summary>
  /// 
  /// </summary>
  public struct MyCulture
  {
    private static readonly IFormatProvider culture = new NumberFormatInfo() { NumberDecimalDigits = '.' };

    public static IFormatProvider Culture
    {
      get
      {
        return culture;
      }
    }
  }
}
