using FileHelpers;
using System;

namespace TestCasesApp
{
  [Serializable]
  [DelimitedRecord(",")]
  public class Instrument : ICloneable
  {
    //public enum EnPriceAlgo
    //{
    //  BEST = 0,
    //  DEPTH = 1,
    //  NCNPV = 2
    //}

    public string Symbol { get; set; }       // name (mt4)

    [FieldHidden]
    public string ShortName { get; set; }        // [SESSION] ShortName

    public string CCYPair { get; set; }      // target symbol (integral)

    //---                    
    public string MinSpread { get; set; }         // minimum spread
    public string BidSpread { get; set; }         // bid spread
    public string AskSpread { get; set; }         // ask spread
    public string MaxSpread { get; set; }         // maximum spread

    [FieldNullValue(typeof(string), " -1")]
    public string MrktRange { get; set; }         // market range

    //--- EnPriceAlgo          
    public string PriceAlgo { get; set; }    // price algo

    [FieldNullValue(typeof(string), " 1M")]
    public string PriceSize { get; set; }         // price size

    [FieldConverter(typeof(BooleanConvertor))]
    [FieldNullValue(typeof(bool), "true")]
    public bool BiasBid { get; set; }             // bid biaz

    [FieldNullValue(typeof(string), " 100K")]
    public string MinQty { get; set; }            // min quantity

    //[FieldConverter(typeof(TimeInForceConvertor))]
    //[FieldNullValue(typeof(char), "6")]

    [FieldNullValue(typeof(string), " GTD")]
    public string TimeInForce { get; set; }         // =FIX::TimeInForce_GOOD_TILL_DATE{ get; set; } 


    public string ExpirationTime { get; set; }       // =30

    public string Enabled { get; set; }             // =false

    //[FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(string), " 1.0")]
    [FieldOptional]
    public string LotConversion { get; set; }     // =1.0  

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    public object Clone()
    {
      var temp = new Instrument()
      {
        AskSpread = this.AskSpread,
        BiasBid = this.BiasBid,
        BidSpread = this.BidSpread,
        CCYPair = this.CCYPair,
        Enabled = this.Enabled,
        ExpirationTime = this.ExpirationTime,
        LotConversion = this.LotConversion,
        MaxSpread = this.MaxSpread,
        MinQty = this.MinQty,
        MinSpread = this.MinSpread,
        MrktRange = this.MrktRange,
        PriceAlgo = this.PriceAlgo,
        PriceSize = this.PriceSize,
        ShortName = this.ShortName,
        Symbol = this.Symbol,
        TimeInForce = this.TimeInForce
      };
      //---
      return temp;
    }
  };
}
