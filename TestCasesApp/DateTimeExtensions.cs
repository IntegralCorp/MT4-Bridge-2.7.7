using System;

namespace TestCasesApp
{
  /// <summary>
  /// datetime extensions
  /// </summary>
  public static class DateTimeExtensions
  {
    /// <summary>
    /// Converts a System.DateTime object to Unix timestamp
    /// </summary>
    /// <returns>The Unix timestamp</returns>
    public static long ConvertToUnixTimestamp(this DateTime date)
    {
      DateTime unixEpoch = new DateTime(1970, 1, 1, 0, 0, 0);
      TimeSpan unixTimeSpan = date - unixEpoch;

      return (long)unixTimeSpan.TotalSeconds;
    }
    /// <summary>
    /// From unix to DateTime format
    /// </summary>
    /// <param name="seconds">количество секунд</param>
    /// <returns>Время</returns>
    public static DateTime ConvertFromUnixTime(this long seconds)
    {
      DateTime origin = new DateTime(1970, 1, 1, 0, 0, 0, 0);
      return origin.AddSeconds(seconds);
    }
    public static DateTime ConvertFromUnixTime(this uint seconds)
    {
      DateTime origin = new DateTime(1970, 1, 1, 0, 0, 0, 0);
      return origin.AddSeconds(seconds);
    }
    public static DateTime ConvertFromUnixTime(this int seconds)
    {
      DateTime origin = new DateTime(1970, 1, 1, 0, 0, 0, 0);
      return origin.AddSeconds(seconds);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="d"></param>
    /// <returns></returns>
    public static bool IsWeekend(this DayOfWeek d)
    {
      return !d.IsWeekday();
    }

    public static bool IsWeekday(this DayOfWeek d)
    {
      switch (d)
      {
        case DayOfWeek.Sunday:
        case DayOfWeek.Saturday: return false;

        default: return true;
      }
    }

    public static DateTime AddWorkdays(this DateTime d, int days)
    {
      // start from a weekday
      while (d.DayOfWeek.IsWeekday()) d = d.AddDays(1.0);
      for (int i = 0; i < days; ++i)
      {
        d = d.AddDays(1.0);
        while (d.DayOfWeek.IsWeekday()) d = d.AddDays(1.0);
      }
      return d;
    }
    public static DateTime PrevWorkday(this DateTime d)
    {
      while (d.DayOfWeek.IsWeekend())
      {
        d = d.AddDays(-1.0);
      }
      //---
      return (d);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="d"></param>
    /// <param name="days"></param>
    /// <returns></returns>
    public static DateTime SubtractWorkdays(this DateTime d, int days)
    {
      if (d.DayOfWeek.IsWeekday()) d = d.AddDays(-1.0);
      // start from a weekday
      while (d.DayOfWeek.IsWeekend())
      {
        d = d.AddDays(-1.0);
      }

      for (int i = 1; i < days; ++i)
      {
        d = d.AddDays(-1.0);
        while (d.DayOfWeek.IsWeekend()) d = d.AddDays(-1.0);
      }
      return d;
    }
    public static DateTime SubtractDays(this DateTime d)
    {
      d = d.AddDays(-1.0);

      return d;
    }
    public static DateTime BeginningOfTheDay(this DateTime date)
    {
      return new DateTime(date.Year, date.Month, date.Day);
    }

    public static DateTime ToDate(this string str)
    {
      if (string.IsNullOrEmpty(str))
      {
        return new DateTime(1970, 1, 1, 0, 0, 0, 0);
      }
      else if (str.Contains(":"))
      {
        return (DateTime.ParseExact(str, "yyyyMMdd HH:mm:ss", MyCulture.Culture));
      }
      else
        return (long.Parse(str)).ConvertFromUnixTime();
    }
  }
}
