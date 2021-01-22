        ��  ��                  \      �� ��     0 	        \4   V S _ V E R S I O N _ I N F O     ���             ?                       �   S t r i n g F i l e I n f o   �   0 4 0 9 0 4 b 0   V   C o m p a n y N a m e     I n t e g r a l   D e v e l o p m e n t   C o r p .     P   F i l e D e s c r i p t i o n     M a r g i n F X   M T 4   B r i d g e   0   F i l e V e r s i o n     2 . 7 . 7 . 0   >   I n t e r n a l N a m e   M T 4 . B r i d g e . i d c     � H  L e g a l C o p y r i g h t   C o p y r i g h t   �   2 0 1 8 - 2 0 2 1 ,   I n t e g r a l   D e v e l o p m e n t   C o r p .     A l l   r i g h t s   r e s e r v e d .   F   O r i g i n a l F i l e n a m e   M T 4 . B r i d g e . i d c     6   P r o d u c t N a m e     M T 4   B r i d g e     4   P r o d u c t V e r s i o n   2 . 7 . 7 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	��= $   X M L   ��g     0 	        <fix major="4" minor="3">
  <header>
    <field name="BeginString" required="Y" />
    <field name="BodyLength" required="Y" />
    <field name="MsgType" required="Y" />
    <field name="SenderCompID" required="Y" />
    <field name="TargetCompID" required="Y" />
    <field name="OnBehalfOfCompID" required="N" />
    <field name="DeliverToCompID" required="N" />
    <field name="SecureDataLen" required="N" />
    <field name="SecureData" required="N" />
    <field name="MsgSeqNum" required="Y" />
    <field name="SenderSubID" required="N" />
    <field name="SenderLocationID" required="N" />
    <field name="TargetSubID" required="N" />
    <field name="TargetLocationID" required="N" />
    <field name="OnBehalfOfSubID" required="N" />
    <field name="OnBehalfOfLocationID" required="N" />
    <field name="DeliverToSubID" required="N" />
    <field name="DeliverToLocationID" required="N" />
    <field name="PossDupFlag" required="N" />
    <field name="PossResend" required="N" />
    <field name="SendingTime" required="Y" />
    <field name="OrigSendingTime" required="N" />
    <field name="XmlDataLen" required="N" />
    <field name="XmlData" required="N" />
    <field name="MessageEncoding" required="N" />
    <field name="LastMsgSeqNumProcessed" required="N" />
    <field name="OnBehalfOfSendingTime" required="N" />
    <group name="NoHops" required="N">
      <field name="HopCompID" required="N" />
      <field name="HopSendingTime" required="N" />    
      <field name="HopRefID" required="N" />
    </group>
  </header>
  <trailer>
    <field name="SignatureLength" required="N" />
    <field name="Signature" required="N" />
    <field name="CheckSum" required="Y" />
  </trailer>
  <messages>
    <message name="Heartbeat" msgtype="0" msgcat="admin">
      <field name="TestReqID" required="N" />
    </message>
    <message name="Logon" msgtype="A" msgcat="admin">
      <field name="EncryptMethod" required="Y" />
      <field name="HeartBtInt" required="Y" />
      <field name="RawDataLength" required="N" />
      <field name="RawData" required="N" />
      <field name="ResetSeqNumFlag" required="N" />
      <field name="MaxMessageSize" required="N" />
      <group name="NoMsgTypes" required="N">
        <field name="RefMsgType" required="N" />
        <field name="MsgDirection" required="N" />
      </group>
      <field name="TestMessageIndicator" required="N" />
      <field name="Username" required="N" />
      <field name="Password" required="N" />
    </message>
    <message name="TestRequest" msgtype="1" msgcat="admin">
      <field name="TestReqID" required="Y" />
    </message>
    <message name="ResendRequest" msgtype="2" msgcat="admin">
      <field name="BeginSeqNo" required="Y" />
      <field name="EndSeqNo" required="Y" />
    </message>
    <message name="Reject" msgtype="3" msgcat="admin">
      <field name="RefSeqNum" required="Y" />
      <field name="RefTagID" required="N" />
      <field name="RefMsgType" required="N" />
      <field name="SessionRejectReason" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="SequenceReset" msgtype="4" msgcat="admin">
      <field name="GapFillFlag" required="N" />
      <field name="NewSeqNo" required="Y" />
    </message>
    <message name="Logout" msgtype="5" msgcat="admin">
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="BusinessMessageReject" msgtype="j" msgcat="admin">
      <field name="RefSeqNum" required="N" />
      <field name="RefMsgType" required="Y" />
      <field name="BusinessRejectRefID" required="N" />
      <field name="BusinessRejectReason" required="Y" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="Advertisement" msgtype="7" msgcat="app">
      <field name="AdvId" required="Y" />
      <field name="AdvTransType" required="Y" />
      <field name="AdvRefID" required="N" />
      <component name="Instrument" required="Y" />
      <field name="AdvSide" required="Y" />
      <field name="Quantity" required="Y" />
      <field name="Price" required="N" />
      <field name="Currency" required="N" />
      <field name="TradeDate" required="N" />
      <field name="TransactTime" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="URLLink" required="N" />
      <field name="LastMkt" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
    </message>
    <message name="IndicationOfInterest" msgtype="6" msgcat="app">
      <field name="IOIid" required="Y" />
      <field name="IOITransType" required="Y" />
      <field name="IOIRefID" required="N" />
      <component name="Instrument" required="Y" />
      <field name="Side" required="Y" />
      <field name="QuantityType" required="N" />
      <field name="IOIQty" required="Y" />
      <field name="PriceType" required="N" />
      <field name="Price" required="N" />
      <field name="Currency" required="N" />
      <field name="ValidUntilTime" required="N" />
      <field name="IOIQltyInd" required="N" />
      <field name="IOINaturalFlag" required="N" />
      <group name="NoIOIQualifiers" required="N">
        <field name="IOIQualifier" required="N" />
      </group>
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="TransactTime" required="N" />
      <field name="URLLink" required="N" />
      <group name="NoRoutingIDs" required="N">
        <field name="RoutingType" required="N" />
        <field name="RoutingID" required="N" />
      </group>
      <component name="SpreadOrBenchmarkCurveData" required="N" />
      <field name="Benchmark" required="N" />
    </message>
    <message name="News" msgtype="B" msgcat="app">
      <field name="OrigTime" required="N" />
      <field name="Urgency" required="N" />
      <field name="Headline" required="Y" />
      <field name="EncodedHeadlineLen" required="N" />
      <field name="EncodedHeadline" required="N" />
      <group name="NoRoutingIDs" required="N">
        <field name="RoutingType" required="N" />
        <field name="RoutingID" required="N" />
      </group>
      <group name="NoRelatedSym" required="N">
        <component name="Instrument" required="N" />
      </group>
      <group name="LinesOfText" required="Y">
        <field name="Text" required="Y" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
      </group>
      <field name="URLLink" required="N" />
      <field name="RawDataLength" required="N" />
      <field name="RawData" required="N" />
    </message>
    <message name="Email" msgtype="C" msgcat="app">
      <field name="EmailThreadID" required="Y" />
      <field name="EmailType" required="Y" />
      <field name="OrigTime" required="N" />
      <field name="Subject" required="Y" />
      <field name="EncodedSubjectLen" required="N" />
      <field name="EncodedSubject" required="N" />
      <group name="NoRoutingIDs" required="N">
        <field name="RoutingType" required="N" />
        <field name="RoutingID" required="N" />
      </group>
      <group name="NoRelatedSym" required="N">
        <component name="Instrument" required="N" />
      </group>
      <field name="OrderID" required="N" />
      <field name="ClOrdID" required="N" />
      <group name="LinesOfText" required="Y">
        <field name="Text" required="Y" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
      </group>
      <field name="RawDataLength" required="N" />
      <field name="RawData" required="N" />
    </message>
    <message name="QuoteRequest" msgtype="R" msgcat="app">
      <field name="QuoteReqID" required="Y" />
      <field name="RFQReqID" required="N" />
      <group name="NoRelatedSym" required="Y">
        <component name="Instrument" required="Y" />
		<field name="SecurityType" required="Y" />
        <field name="PrevClosePx" required="N" />
        <field name="QuoteRequestType" required="N" />
        <field name="QuoteType" required="N" />
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
        <field name="TradeOriginationDate" required="N" />
        <component name="Stipulations" required="N" />
        <field name="Side" required="N" />
        <field name="QuantityType" required="N" />
        <field name="OrderQty" required="N" />
        <field name="CashOrderQty" required="N" />
        <field name="SettlmntTyp" required="N" />
        <field name="FutSettDate" required="N" />
		<field name="OrdType" required="Y" />
        <field name="FutSettDate2" required="N" />
        <field name="OrderQty2" required="N" />
        <field name="ExpireTime" required="N" />
        <field name="TransactTime" required="N" />
        <field name="Currency" required="N" />
        <component name="SpreadOrBenchmarkCurveData" required="N" />
        <field name="PriceType" required="N" />
        <field name="Price" required="N" />
        <field name="Price2" required="N" />
        <component name="YieldData" required="N" />
      </group>
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="QuoteRequestReject" msgtype="AG" msgcat="app">
      <field name="QuoteReqID" required="Y" />
      <field name="RFQReqID" required="N" />
      <field name="QuoteRequestRejectReason" required="Y" />
	  <field name="Text" required="N" />
      <group name="NoRelatedSym" required="N">
        <component name="Instrument" required="N" />
        <field name="PrevClosePx" required="N" />
        <field name="QuoteRequestType" required="N" />
        <field name="QuoteType" required="N" />
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
        <field name="TradeOriginationDate" required="N" />
        <component name="Stipulations" required="N" />
        <field name="Side" required="N" />
        <field name="QuantityType" required="N" />
        <field name="OrderQty" required="N" />
        <field name="CashOrderQty" required="N" />
        <field name="SettlmntTyp" required="N" />
        <field name="FutSettDate" required="N" />
        <field name="OrdType" required="N" />
        <field name="FutSettDate2" required="N" />
        <field name="OrderQty2" required="N" />
        <field name="ExpireTime" required="N" />
		<field name="TransactTime" required="N" />
        <field name="Currency" required="N" />
        <component name="SpreadOrBenchmarkCurveData" required="N" />
        <field name="PriceType" required="N" />
        <field name="Price" required="N" />
        <field name="Price2" required="N" />
        <component name="YieldData" required="N" />
      </group>
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="RFQRequest" msgtype="AH" msgcat="app">
      <field name="RFQReqID" required="Y" />
      <group name="NoRelatedSym" required="Y">
        <component name="Instrument" required="Y" />
        <field name="PrevClosePx" required="N" />
        <field name="QuoteRequestType" required="N" />
        <field name="QuoteType" required="N" />
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
      </group>
      <field name="SubscriptionRequestType" required="N" />
    </message>
    <message name="Quote" msgtype="S" msgcat="app">
      <field name="QuoteReqID" required="N" />
      <field name="QuoteID" required="Y" />
      <field name="QuoteType" required="N" />
      <field name="QuoteResponseLevel" required="N" />
      <component name="Parties" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <component name="Instrument" required="Y" />
      <field name="BidPx" required="N" />
      <field name="OfferPx" required="N" />
      <field name="MktBidPx" required="N" />
      <field name="MktOfferPx" required="N" />
      <field name="MinBidSize" required="N" />
      <field name="BidSize" required="N" />
	  <field name="BidSize2" required="N" />	  
      <field name="MinOfferSize" required="N" />
      <field name="OfferSize" required="N" />
	  <field name="OfferSize2" required="N" />
      <field name="ValidUntilTime" required="N" />
      <field name="BidSpotRate" required="N" />
      <field name="OfferSpotRate" required="N" />
      <field name="BidForwardPoints" required="N" />
      <field name="OfferForwardPoints" required="N" />
      <field name="MidPx" required="N" />
      <field name="BidYield" required="N" />
      <field name="MidYield" required="N" />
      <field name="OfferYield" required="N" />
      <field name="TransactTime" required="N" />
      <field name="SettlmntTyp" required="N" />
      <field name="FutSettDate" required="N" />
      <field name="OrdType" required="N" />
      <field name="FutSettDate2" required="N" />
      <field name="OrderQty2" required="N" />
      <field name="BidForwardPoints2" required="N" />
      <field name="OfferForwardPoints2" required="N" />
      <field name="Currency" required="N" />
      <field name="SettlCurrBidFxRate" required="N" />
      <field name="SettlCurrOfferFxRate" required="N" />
      <field name="SettlCurrFxRateCalc" required="N" />
      <field name="CommType" required="N" />
      <field name="Commission" required="N" />
      <field name="CustOrderCapacity" required="N" />
      <field name="ExDestination" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
	  <field name="Side" required="Y" />
    </message>
    <message name="QuoteCancel" msgtype="Z" msgcat="app">
      <field name="QuoteReqID" required="N" />
      <field name="QuoteID" required="N" />
      <field name="QuoteCancelType" required="Y" />
      <field name="QuoteResponseLevel" required="N" />
	  <field name="TransactTime" required="Y" />
      <component name="Parties" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <group name="NoQuoteEntries" required="N">
        <component name="Instrument" required="N" />
      </group>
    </message>
    <message name="QuoteStatusRequest" msgtype="a" msgcat="app">
      <field name="QuoteStatusReqID" required="N" />
      <field name="QuoteID" required="N" />
      <component name="Instrument" required="Y" />
      <component name="Parties" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <field name="SubscriptionRequestType" required="N" />
    </message>
    <message name="QuoteStatusReport" msgtype="AI" msgcat="app">
      <field name="QuoteStatusReqID" required="N" />
      <field name="QuoteReqID" required="N" />
      <field name="QuoteID" required="Y" />
      <field name="QuoteType" required="N" />
      <component name="Parties" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <component name="Instrument" required="Y" />
      <field name="BidPx" required="N" />
      <field name="OfferPx" required="N" />
      <field name="MktBidPx" required="N" />
      <field name="MktOfferPx" required="N" />
      <field name="MinBidSize" required="N" />
      <field name="BidSize" required="N" />
      <field name="MinOfferSize" required="N" />
      <field name="OfferSize" required="N" />
      <field name="ValidUntilTime" required="N" />
      <field name="BidSpotRate" required="N" />
      <field name="OfferSpotRate" required="N" />
      <field name="BidForwardPoints" required="N" />
      <field name="OfferForwardPoints" required="N" />
      <field name="MidPx" required="N" />
      <field name="BidYield" required="N" />
      <field name="MidYield" required="N" />
      <field name="OfferYield" required="N" />
      <field name="TransactTime" required="N" />
      <field name="FutSettDate" required="N" />
      <field name="OrdType" required="N" />
      <field name="FutSettDate2" required="N" />
      <field name="OrderQty2" required="N" />
      <field name="BidForwardPoints2" required="N" />
      <field name="OfferForwardPoints2" required="N" />
      <field name="Currency" required="N" />
      <field name="SettlCurrBidFxRate" required="N" />
      <field name="SettlCurrOfferFxRate" required="N" />
      <field name="SettlCurrFxRateCalc" required="N" />
      <field name="CommType" required="N" />
      <field name="Commission" required="N" />
      <field name="CustOrderCapacity" required="N" />
      <field name="ExDestination" required="N" />
      <field name="QuoteStatus" required="N" />
    </message>
    <message name="MassQuote" msgtype="i" msgcat="app">
      <field name="QuoteReqID" required="N" />
      <field name="QuoteID" required="Y" />
      <field name="QuoteType" required="N" />
      <field name="QuoteResponseLevel" required="N" />
      <component name="Parties" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <field name="DefBidSize" required="N" />
      <field name="DefOfferSize" required="N" />
      <group name="NoQuoteSets" required="Y">
        <field name="QuoteSetID" required="Y" />
        <component name="UnderlyingInstrument" required="N" />
        <field name="QuoteSetValidUntilTime" required="N" />
        <field name="TotQuoteEntries" required="Y" />
        <group name="NoQuoteEntries" required="N">
          <field name="QuoteEntryID" required="Y" />
          <component name="Instrument" required="N" />
          <field name="BidPx" required="N" />
          <field name="OfferPx" required="N" />
          <field name="BidSize" required="N" />
          <field name="OfferSize" required="N" />
          <field name="ValidUntilTime" required="N" />
          <field name="BidSpotRate" required="N" />
          <field name="OfferSpotRate" required="N" />
          <field name="BidForwardPoints" required="N" />
          <field name="OfferForwardPoints" required="N" />
          <field name="MidPx" required="N" />
          <field name="BidYield" required="N" />
          <field name="MidYield" required="N" />
          <field name="OfferYield" required="N" />
          <field name="TransactTime" required="N" />
          <field name="TradingSessionID" required="N" />
          <field name="TradingSessionSubID" required="N" />
          <field name="FutSettDate" required="N" />
          <field name="OrdType" required="N" />
          <field name="FutSettDate2" required="N" />
          <field name="OrderQty2" required="N" />
          <field name="BidForwardPoints2" required="N" />
          <field name="OfferForwardPoints2" required="N" />
          <field name="Currency" required="N" />
        </group>
      </group>
    </message>
    <message name="MassQuoteAcknowledgement" msgtype="b" msgcat="app">
      <field name="QuoteReqID" required="N" />
      <field name="QuoteID" required="N" />
      <field name="QuoteStatus" required="Y" />
      <field name="QuoteRejectReason" required="N" />
      <field name="QuoteResponseLevel" required="N" />
      <field name="QuoteType" required="N" />
      <component name="Parties" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <field name="Text" required="N" />
      <group name="NoQuoteSets" required="N">
        <field name="QuoteSetID" required="N" />
        <component name="UnderlyingInstrument" required="N" />
        <field name="TotQuoteEntries" required="N" />
        <group name="NoQuoteEntries" required="N">
          <field name="QuoteEntryID" required="N" />
          <component name="Instrument" required="N" />
          <field name="BidPx" required="N" />
          <field name="OfferPx" required="N" />
          <field name="BidSize" required="N" />
          <field name="OfferSize" required="N" />
          <field name="ValidUntilTime" required="N" />
          <field name="BidSpotRate" required="N" />
          <field name="OfferSpotRate" required="N" />
          <field name="BidForwardPoints" required="N" />
          <field name="OfferForwardPoints" required="N" />
          <field name="MidPx" required="N" />
          <field name="BidYield" required="N" />
          <field name="MidYield" required="N" />
          <field name="OfferYield" required="N" />
          <field name="TransactTime" required="N" />
          <field name="TradingSessionID" required="N" />
          <field name="TradingSessionSubID" required="N" />
          <field name="FutSettDate" required="N" />
          <field name="OrdType" required="N" />
          <field name="FutSettDate2" required="N" />
          <field name="OrderQty2" required="N" />
          <field name="BidForwardPoints2" required="N" />
          <field name="OfferForwardPoints2" required="N" />
          <field name="Currency" required="N" />
          <field name="QuoteEntryRejectReason" required="N" />
        </group>
      </group>
    </message>
    <message name="MarketDataRequest" msgtype="V" msgcat="app">
      <field name="MDReqID" required="Y" />
      <field name="SubscriptionRequestType" required="Y" />
      <field name="MarketDepth" required="Y" />
      <field name="MDUpdateType" required="Y" />
      <field name="AggregatedBook" required="N" />
      <field name="OpenCloseSettleFlag" required="N" />
      <field name="Scope" required="N" />
      <field name="MDImplicitDelete" required="N" />
	  <field name="RequestedSize" required="N" />
      <group name="NoMDEntryTypes" required="Y">
        <field name="MDEntryType" required="Y" />
      </group>
      <group name="NoRelatedSym" required="Y">
        <component name="Instrument" required="Y" />
      </group>
      <group name="NoTradingSessions" required="N">
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
      </group>
    </message>
    <message name="MarketDataSnapshotFullRefresh" msgtype="W" msgcat="app">
      <field name="MDReqID" required="Y" />
      <component name="Instrument" required="Y" />
      <field name="FinancialStatus" required="N" />
      <field name="CorporateAction" required="N" />
      <field name="TotalVolumeTraded" required="N" />
      <field name="TotalVolumeTradedDate" required="N" />
      <field name="TotalVolumeTradedTime" required="N" />
      <field name="NetChgPrevDay" required="N" />
      <group name="NoMDEntries" required="Y">
        <field name="MDEntryType" required="Y" />
        <field name="MDEntryPx" required="Y" />
        <field name="Currency" required="Y" />
        <field name="MDEntrySize" required="Y" />
        <field name="MDEntryDate" required="N" />
        <field name="MDEntryTime" required="N" />
        <field name="TickDirection" required="N" />
        <field name="MDMkt" required="N" />
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
        <field name="QuoteCondition" required="N" />
        <field name="TradeCondition" required="N" />
        <field name="MDEntryOriginator" required="Y" />
        <field name="LocationID" required="N" />
        <field name="DeskID" required="N" />
        <field name="OpenCloseSettleFlag" required="N" />
        <field name="TimeInForce" required="N" />
        <field name="ExpireDate" required="N" />
        <field name="ExpireTime" required="N" />
        <field name="MinQty" required="N" />
        <field name="ExecInst" required="N" />
        <field name="SellerDays" required="N" />
        <field name="OrderID" required="N" />
        <field name="QuoteEntryID" required="Y" />
        <field name="MDEntryBuyer" required="N" />
        <field name="MDEntrySeller" required="N" />
        <field name="NumberOfOrders" required="N" />
        <field name="MDEntryPositionNo" required="N" />
        <field name="Scope" required="N" />
        <field name="Text" required="N" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
      </group>
    </message>
    <message name="MarketDataIncrementalRefresh" msgtype="X" msgcat="app">
      <field name="MDReqID" required="N" />
      <group name="NoMDEntries" required="Y">
        <field name="MDUpdateAction" required="Y" />
        <field name="DeleteReason" required="N" />
        <field name="MDEntryType" required="N" />
        <field name="MDEntryID" required="N" />
        <field name="MDEntryRefID" required="N" />
        <component name="Instrument" required="N" />
        <field name="FinancialStatus" required="N" />
        <field name="CorporateAction" required="N" />
        <field name="MDEntryPx" required="N" />
        <field name="Currency" required="N" />
        <field name="MDEntrySize" required="N" />
        <field name="MDEntryDate" required="N" />
        <field name="MDEntryTime" required="N" />
        <field name="TickDirection" required="N" />
        <field name="MDMkt" required="N" />
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
        <field name="QuoteCondition" required="N" />
        <field name="TradeCondition" required="N" />
        <field name="MDEntryOriginator" required="N" />
        <field name="LocationID" required="N" />
        <field name="DeskID" required="N" />
        <field name="OpenCloseSettleFlag" required="N" />
        <field name="TimeInForce" required="N" />
        <field name="ExpireDate" required="N" />
        <field name="ExpireTime" required="N" />
        <field name="MinQty" required="N" />
        <field name="ExecInst" required="N" />
        <field name="SellerDays" required="N" />
        <field name="OrderID" required="N" />
        <field name="QuoteEntryID" required="N" />
        <field name="MDEntryBuyer" required="N" />
        <field name="MDEntrySeller" required="N" />
        <field name="NumberOfOrders" required="N" />
        <field name="MDEntryPositionNo" required="N" />
        <field name="Scope" required="N" />
        <field name="TotalVolumeTraded" required="N" />
        <field name="TotalVolumeTradedDate" required="N" />
        <field name="TotalVolumeTradedTime" required="N" />
        <field name="NetChgPrevDay" required="N" />
        <field name="Text" required="N" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
      </group>
    </message>
    <message name="MarketDataRequestReject" msgtype="Y" msgcat="app">
      <field name="MDReqID" required="Y" />
      <field name="MDReqRejReason" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="SecurityDefinitionRequest" msgtype="c" msgcat="app">
      <field name="SecurityReqID" required="Y" />
      <field name="SecurityRequestType" required="Y" />
      <component name="Instrument" required="N" />
      <field name="Currency" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <group name="NoLegs" required="N">
        <component name="InstrumentLeg" required="N" />
        <field name="LegCurrency" required="N" />
      </group>
      <field name="SubscriptionRequestType" required="N" />
    </message>
    <message name="SecurityDefinition" msgtype="d" msgcat="app">
      <field name="SecurityReqID" required="Y" />
      <field name="SecurityResponseID" required="Y" />
      <field name="SecurityResponseType" required="Y" />
      <component name="Instrument" required="N" />
      <field name="Currency" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <group name="NoLegs" required="N">
        <component name="InstrumentLeg" required="N" />
        <field name="LegCurrency" required="N" />
      </group>
      <field name="RoundLot" required="N" />
      <field name="MinTradeVol" required="N" />
    </message>
    <message name="SecurityTypeRequest" msgtype="v" msgcat="app">
      <field name="SecurityReqID" required="Y" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
    </message>
    <message name="SecurityTypes" msgtype="w" msgcat="app">
      <field name="SecurityReqID" required="Y" />
      <field name="SecurityResponseID" required="Y" />
      <field name="SecurityResponseType" required="Y" />
      <field name="TotalNumSecurityTypes" required="N" />
      <group name="NoSecurityTypes" required="N">
        <field name="SecurityType" required="N" />
        <field name="Product" required="N" />
        <field name="CFICode" required="N" />
      </group>
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <field name="SubscriptionRequestType" required="N" />
    </message>
    <message name="SecurityListRequest" msgtype="x" msgcat="app">
      <field name="SecurityReqID" required="Y" />
      <field name="SecurityListRequestType" required="Y" />
      <component name="Instrument" required="N" />
      <field name="Currency" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <field name="SubscriptionRequestType" required="N" />
    </message>
    <message name="SecurityList" msgtype="y" msgcat="app">
      <field name="SecurityReqID" required="Y" />
      <field name="SecurityResponseID" required="Y" />
      <field name="SecurityRequestResult" required="Y" />
      <field name="TotalNumSecurities" required="N" />
      <group name="NoRelatedSym" required="N">
        <component name="Instrument" required="N" />
        <field name="Currency" required="N" />
        <group name="NoLegs" required="N">
          <component name="InstrumentLeg" required="N" />
          <field name="LegCurrency" required="N" />
        </group>
        <field name="RoundLot" required="N" />
        <field name="MinTradeVol" required="N" />
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
        <field name="Text" required="N" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
      </group>
    </message>
    <message name="DerivativeSecurityListRequest" msgtype="z" msgcat="app">
      <field name="SecurityReqID" required="Y" />
      <field name="SecurityListRequestType" required="Y" />
      <component name="UnderlyingInstrument" required="N" />
      <field name="Currency" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <field name="SubscriptionRequestType" required="N" />
    </message>
    <message name="DerivativeSecurityList" msgtype="AA" msgcat="app">
      <field name="SecurityReqID" required="Y" />
      <field name="SecurityResponseID" required="Y" />
      <field name="SecurityRequestResult" required="Y" />
      <component name="UnderlyingInstrument" required="N" />
      <field name="TotalNumSecurities" required="N" />
      <group name="NoRelatedSym" required="N">
        <component name="Instrument" required="N" />
        <field name="Currency" required="N" />
        <group name="NoLegs" required="N">
          <component name="InstrumentLeg" required="N" />
          <field name="LegCurrency" required="N" />
        </group>
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
        <field name="Text" required="N" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
      </group>
    </message>
    <message name="SecurityStatusRequest" msgtype="e" msgcat="app">
      <field name="SecurityStatusReqID" required="Y" />
      <component name="Instrument" required="Y" />
      <field name="Currency" required="N" />
      <field name="SubscriptionRequestType" required="Y" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
    </message>
    <message name="SecurityStatus" msgtype="f" msgcat="app">
      <field name="SecurityStatusReqID" required="N" />
      <component name="Instrument" required="Y" />
      <field name="Currency" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <field name="UnsolicitedIndicator" required="N" />
      <field name="SecurityTradingStatus" required="N" />
      <field name="FinancialStatus" required="N" />
      <field name="CorporateAction" required="N" />
      <field name="HaltReason" required="N" />
      <field name="InViewOfCommon" required="N" />
      <field name="DueToRelated" required="N" />
      <field name="BuyVolume" required="N" />
      <field name="SellVolume" required="N" />
      <field name="HighPx" required="N" />
      <field name="LowPx" required="N" />
      <field name="LastPx" required="N" />
      <field name="TransactTime" required="N" />
      <field name="Adjustment" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="TradingSessionStatusRequest" msgtype="g" msgcat="app">
      <field name="TradSesReqID" required="Y" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <field name="TradSesMethod" required="N" />
      <field name="TradSesMode" required="N" />
      <field name="SubscriptionRequestType" required="Y" />
    </message>
    <message name="TradingSessionStatus" msgtype="h" msgcat="app">
      <field name="TradSesReqID" required="N" />
      <field name="TradingSessionID" required="Y" />
      <field name="TradingSessionSubID" required="N" />
      <field name="TradSesMethod" required="N" />
      <field name="TradSesMode" required="N" />
      <field name="UnsolicitedIndicator" required="N" />
      <field name="TradSesStatus" required="Y" />
      <field name="TradSesStatusRejReason" required="N" />
      <field name="TradSesStartTime" required="N" />
      <field name="TradSesOpenTime" required="N" />
      <field name="TradSesPreCloseTime" required="N" />
      <field name="TradSesCloseTime" required="N" />
      <field name="TradSesEndTime" required="N" />
      <field name="TotalVolumeTraded" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="NewOrderSingle" msgtype="D" msgcat="app">
      <field name="ClOrdID" required="Y" />
      <field name="SecondaryClOrdID" required="N" />
      <field name="ClOrdLinkID" required="N" />
      <component name="Parties" required="N" />
      <field name="TradeOriginationDate" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <field name="DayBookingInst" required="N" />
      <field name="BookingUnit" required="N" />
      <field name="PreallocMethod" required="N" />
      <group name="NoAllocs" required="N">
        <field name="AllocAccount" required="N" />
        <field name="IndividualAllocID" required="N" />
        <component name="NestedParties" required="N" />
        <field name="AllocQty" required="N" />
      </group>
      <field name="SettlmntTyp" required="N" />
      <field name="FutSettDate" required="N" />
      <field name="CashMargin" required="N" />
      <field name="ClearingFeeIndicator" required="N" />
      <field name="HandlInst" required="Y" />
      <field name="ExecInst" required="N" />
      <field name="MinQty" required="N" />
      <field name="MaxFloor" required="N" />
      <field name="ExDestination" required="N" />
      <group name="NoTradingSessions" required="N">
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
      </group>
      <field name="ProcessCode" required="N" />
      <component name="Instrument" required="Y" />
      <field name="PrevClosePx" required="N" />
      <field name="Side" required="Y" />
      <field name="LocateReqd" required="N" />
      <field name="TransactTime" required="Y" />
      <component name="Stipulations" required="N" />
      <field name="QuantityType" required="N" />
      <component name="OrderQtyData" required="Y" />
      <field name="OrdType" required="Y" />
      <field name="PriceType" required="N" />
      <field name="Price" required="N" />
      <field name="StopPx" required="N" />
      <component name="SpreadOrBenchmarkCurveData" required="N" />
      <component name="YieldData" required="N" />
      <field name="Currency" required="Y" />
      <field name="ComplianceID" required="N" />
      <field name="SolicitedFlag" required="N" />
      <field name="IOIid" required="N" />
      <field name="QuoteID" required="N" />
      <field name="TimeInForce" required="N" />
      <field name="EffectiveTime" required="N" />
      <field name="ExpireDate" required="N" />
      <field name="ExpireTime" required="N" />
      <field name="GTBookingInst" required="N" />
      <component name="CommissionData" required="N" />
      <field name="OrderCapacity" required="N" />
      <field name="OrderRestrictions" required="N" />
      <field name="CustOrderCapacity" required="N" />
      <field name="Rule80A" required="N" />
      <field name="ForexReq" required="N" />
      <field name="SettlCurrency" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="FutSettDate2" required="N" />
      <field name="OrderQty2" required="N" />
      <field name="Price2" required="N" />
      <field name="PositionEffect" required="N" />
      <field name="CoveredOrUncovered" required="N" />
      <field name="MaxShow" required="N" />
      <field name="PegDifference" required="N" />
      <field name="DiscretionInst" required="N" />
      <field name="DiscretionOffset" required="N" />
      <field name="CancellationRights" required="N" />
      <field name="MoneyLaunderingStatus" required="N" />
      <field name="RegistID" required="N" />
      <field name="Designation" required="N" />
      <field name="AccruedInterestRate" required="N" />
      <field name="AccruedInterestAmt" required="N" />
      <field name="NetMoney" required="N" />
	  <field name="MarketRangeABS" required="N" />
	  <field name="AdditionalExecInst" required="N" />
    </message>
    <message name="ExecutionReport" msgtype="8" msgcat="app">
      <field name="OrderID" required="Y" />
      <field name="SecondaryOrderID" required="N" />
      <field name="SecondaryClOrdID" required="N" />
      <field name="SecondaryExecID" required="N" />
      <field name="ClOrdID" required="Y" />
	  <field name="OrdStatusReqID" required="N" />
      <field name="OrigClOrdID" required="N" />
      <field name="ClOrdLinkID" required="N" />
	  <field name="MassStatusReqID" required="N" />
      <component name="Parties" required="N" />
      <field name="TradeOriginationDate" required="N" />
      <group name="NoContraBrokers" required="N">
        <field name="ContraBroker" required="N" />
        <field name="ContraTrader" required="N" />
        <field name="ContraTradeQty" required="N" />
        <field name="ContraTradeTime" required="N" />
        <field name="ContraLegRefID" required="N" />
      </group>
	  <field name="SecurityType" required="N" />
	  <field name="Price2" required="N" />
	  <field name="LastPx2" required="N" />
	  <field name="LastSpotRate2" required="N" />
	  <field name="LeavesQty2" required="N" />
	  <field name="CumQty2" required="N" />
	  <field name="SettlCurrAmt2" required="N" />
      <field name="ListID" required="N" />
      <field name="CrossID" required="N" />
      <field name="OrigCrossID" required="N" />
      <field name="CrossType" required="N" />
      <field name="ExecID" required="Y" />
      <field name="ExecRefID" required="N" />
      <field name="ExecType" required="Y" />
      <field name="OrdStatus" required="Y" />
      <field name="WorkingIndicator" required="N" />
      <field name="OrdRejReason" required="N" />
      <field name="ExecRestatementReason" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <field name="DayBookingInst" required="N" />
      <field name="BookingUnit" required="N" />
      <field name="PreallocMethod" required="N" />
      <field name="SettlmntTyp" required="N" />
      <field name="FutSettDate" required="N" />
      <field name="CashMargin" required="N" />
      <field name="ClearingFeeIndicator" required="N" />
      <component name="Instrument" required="Y" />
      <field name="Side" required="Y" />
      <component name="Stipulations" required="N" />
      <field name="QuantityType" required="N" />
      <component name="OrderQtyData" required="Y" />
      <field name="OrdType" required="N" />
      <field name="PriceType" required="N" />
      <field name="Price" required="N" />
      <field name="StopPx" required="N" />
      <field name="PegDifference" required="N" />
      <field name="DiscretionInst" required="N" />
      <field name="DiscretionOffset" required="N" />
      <field name="Currency" required="N" />
      <field name="ComplianceID" required="N" />
      <field name="SolicitedFlag" required="N" />
      <field name="TimeInForce" required="N" />
      <field name="EffectiveTime" required="N" />
      <field name="ExpireDate" required="N" />
      <field name="ExpireTime" required="N" />
      <field name="ExecInst" required="N" />
      <field name="OrderCapacity" required="N" />
      <field name="OrderRestrictions" required="N" />
      <field name="CustOrderCapacity" required="N" />
      <field name="Rule80A" required="N" />
      <field name="LastQty" required="N" />
      <field name="UnderlyingLastQty" required="N" />
      <field name="LastPx" required="N" />
      <field name="UnderlyingLastPx" required="N" />
      <field name="LastSpotRate" required="N" />
      <field name="LastForwardPoints" required="N" />
      <field name="LastMkt" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <field name="LastCapacity" required="N" />
      <field name="LeavesQty" required="Y" />
      <field name="CumQty" required="Y" />
      <field name="AvgPx" required="Y" />
      <field name="DayOrderQty" required="N" />
      <field name="DayCumQty" required="N" />
      <field name="DayAvgPx" required="N" />
      <field name="GTBookingInst" required="N" />
      <field name="TradeDate" required="N" />
      <field name="TransactTime" required="Y" />
      <field name="ReportToExch" required="N" />
      <component name="CommissionData" required="N" />
      <component name="SpreadOrBenchmarkCurveData" required="N" />
      <component name="YieldData" required="N" />
      <field name="GrossTradeAmt" required="N" />
      <field name="NumDaysInterest" required="N" />
      <field name="ExDate" required="N" />
      <field name="AccruedInterestRate" required="N" />
      <field name="AccruedInterestAmt" required="N" />
      <field name="TradedFlatSwitch" required="N" />
      <field name="BasisFeatureDate" required="N" />
      <field name="BasisFeaturePrice" required="N" />
      <field name="Concession" required="N" />
      <field name="TotalTakedown" required="N" />
      <field name="NetMoney" required="N" />
      <field name="SettlCurrAmt" required="N" />
      <field name="SettlCurrency" required="N" />
      <field name="SettlCurrFxRate" required="N" />
      <field name="SettlCurrFxRateCalc" required="N" />
      <field name="HandlInst" required="N" />
      <field name="MinQty" required="N" />
      <field name="MaxFloor" required="N" />
      <field name="PositionEffect" required="N" />
      <field name="MaxShow" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="FutSettDate2" required="N" />
      <field name="OrderQty2" required="N" />
      <field name="LastForwardPoints2" required="N" />
      <field name="MultiLegReportingType" required="N" />
      <field name="CancellationRights" required="N" />
      <field name="MoneyLaunderingStatus" required="N" />
      <field name="RegistID" required="N" />
      <field name="Designation" required="N" />
      <field name="TransBkdTime" required="N" />
      <field name="ExecValuationPoint" required="N" />
      <field name="ExecPriceType" required="N" />
      <field name="ExecPriceAdjustment" required="N" />
      <field name="PriorityIndicator" required="N" />
      <field name="PriceImprovement" required="N" />
      <group name="NoContAmts" required="N">
        <field name="ContAmtType" required="N" />
        <field name="ContAmtValue" required="N" />
        <field name="ContAmtCurr" required="N" />
      </group>
      <group name="NoLegs" required="N">
        <component name="InstrumentLeg" required="N" />
        <field name="LegPositionEffect" required="N" />
        <field name="LegCoveredOrUncovered" required="N" />
        <component name="NestedParties" required="N" />
        <field name="LegRefID" required="N" />
        <field name="LegPrice" required="N" />
        <field name="LegSettlmntTyp" required="N" />
        <field name="LegFutSettDate" required="N" />
        <field name="LegLastPx" required="N" />
      </group>
    </message>
    <message name="DontKnowTrade" msgtype="Q" msgcat="app">
      <field name="OrderID" required="Y" />
      <field name="ExecID" required="Y" />
      <field name="DKReason" required="Y" />
      <component name="Instrument" required="Y" />
      <field name="Side" required="Y" />
      <component name="OrderQtyData" required="Y" />
      <field name="LastQty" required="N" />
      <field name="LastPx" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="OrderCancelReplaceRequest" msgtype="G" msgcat="app">
      <field name="OrderID" required="N" />
      <component name="Parties" required="N" />
      <field name="TradeOriginationDate" required="N" />
      <field name="OrigClOrdID" required="Y" />
      <field name="ClOrdID" required="Y" />
      <field name="SecondaryClOrdID" required="N" />
      <field name="ClOrdLinkID" required="N" />
      <field name="ListID" required="N" />
      <field name="OrigOrdModTime" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <field name="DayBookingInst" required="N" />
      <field name="BookingUnit" required="N" />
      <field name="PreallocMethod" required="N" />
      <group name="NoAllocs" required="N">
        <field name="AllocAccount" required="N" />
        <field name="IndividualAllocID" required="N" />
        <component name="NestedParties" required="N" />
        <field name="AllocQty" required="N" />
      </group>
      <field name="SettlmntTyp" required="N" />
      <field name="FutSettDate" required="N" />
      <field name="CashMargin" required="N" />
      <field name="ClearingFeeIndicator" required="N" />
      <field name="HandlInst" required="Y" />
      <field name="ExecInst" required="N" />
      <field name="MinQty" required="N" />
      <field name="MaxFloor" required="N" />
      <field name="ExDestination" required="N" />
      <group name="NoTradingSessions" required="N">
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
      </group>
      <component name="Instrument" required="Y" />
      <field name="Side" required="Y" />
      <field name="TransactTime" required="Y" />
      <field name="QuantityType" required="N" />
      <component name="OrderQtyData" required="Y" />
      <field name="OrdType" required="Y" />
      <field name="PriceType" required="N" />
      <field name="Price" required="N" />
      <field name="StopPx" required="N" />
      <component name="SpreadOrBenchmarkCurveData" required="N" />
      <component name="YieldData" required="N" />
      <field name="PegDifference" required="N" />
      <field name="DiscretionInst" required="N" />
      <field name="DiscretionOffset" required="N" />
      <field name="ComplianceID" required="N" />
      <field name="SolicitedFlag" required="N" />
      <field name="Currency" required="N" />
      <field name="TimeInForce" required="N" />
      <field name="EffectiveTime" required="N" />
      <field name="ExpireDate" required="N" />
      <field name="ExpireTime" required="N" />
      <field name="GTBookingInst" required="N" />
      <component name="CommissionData" required="N" />
      <field name="OrderCapacity" required="N" />
      <field name="OrderRestrictions" required="N" />
      <field name="CustOrderCapacity" required="N" />
      <field name="Rule80A" required="N" />
      <field name="ForexReq" required="N" />
      <field name="SettlCurrency" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="FutSettDate2" required="N" />
      <field name="OrderQty2" required="N" />
      <field name="Price2" required="N" />
      <field name="PositionEffect" required="N" />
      <field name="CoveredOrUncovered" required="N" />
      <field name="MaxShow" required="N" />
      <field name="LocateReqd" required="N" />
      <field name="CancellationRights" required="N" />
      <field name="MoneyLaunderingStatus" required="N" />
      <field name="RegistID" required="N" />
      <field name="Designation" required="N" />
      <field name="AccruedInterestRate" required="N" />
      <field name="AccruedInterestAmt" required="N" />
      <field name="NetMoney" required="N" />
	  <field name="MarketRangeABS" required="N" />
	  <field name="AdditionalExecInst" required="N" />
    </message>
    <message name="OrderCancelRequest" msgtype="F" msgcat="app">
      <field name="OrigClOrdID" required="Y" />
      <field name="OrderID" required="N" />
      <field name="ClOrdID" required="Y" />
      <field name="SecondaryClOrdID" required="N" />
      <field name="ClOrdLinkID" required="N" />
      <field name="ListID" required="N" />
      <field name="OrigOrdModTime" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <component name="Parties" required="N" />
      <component name="Instrument" required="Y" />
      <field name="Side" required="Y" />
      <field name="TransactTime" required="Y" />
      <component name="OrderQtyData" required="Y" />
      <field name="ComplianceID" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="OrderCancelReject" msgtype="9" msgcat="app">
      <field name="OrderID" required="Y" />
      <field name="SecondaryOrderID" required="N" />
      <field name="SecondaryClOrdID" required="N" />
      <field name="ClOrdID" required="Y" />
      <field name="ClOrdLinkID" required="N" />
      <field name="OrigClOrdID" required="Y" />
      <field name="OrdStatus" required="Y" />
      <field name="WorkingIndicator" required="N" />
      <field name="OrigOrdModTime" required="N" />
      <field name="ListID" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <field name="TradeOriginationDate" required="N" />
      <field name="TransactTime" required="N" />
      <field name="CxlRejResponseTo" required="Y" />
      <field name="CxlRejReason" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="OrderStatusRequest" msgtype="H" msgcat="app">
      <field name="OrderID" required="N" />
      <field name="ClOrdID" required="Y" />
      <field name="SecondaryClOrdID" required="N" />
      <field name="ClOrdLinkID" required="N" />
      <component name="Parties" required="N" />
      <field name="Account" required="N" />
      <component name="Instrument" required="Y" />
      <field name="Side" required="Y" />
    </message>
    <message name="OrderMassCancelRequest" msgtype="q" msgcat="app">
      <field name="ClOrdID" required="Y" />
      <field name="SecondaryClOrdID" required="N" />
      <field name="MassCancelRequestType" required="Y" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <component name="Instrument" required="N" />
      <component name="UnderlyingInstrument" required="N" />
      <field name="Side" required="N" />
      <field name="TransactTime" required="Y" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="OrderMassCancelReport" msgtype="r" msgcat="app">
      <field name="ClOrdID" required="N" />
      <field name="SecondaryClOrdID" required="N" />
      <field name="OrderID" required="Y" />
      <field name="SecondaryOrderID" required="N" />
      <field name="MassCancelRequestType" required="Y" />
      <field name="MassCancelResponse" required="Y" />
      <field name="MassCancelRejectReason" required="N" />
      <field name="TotalAffectedOrders" required="N" />
      <group name="NoAffectedOrders" required="N">
        <field name="OrigClOrdID" required="N" />
        <field name="AffectedOrderID" required="N" />
        <field name="AffectedSecondaryOrderID" required="N" />
      </group>
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <component name="Instrument" required="N" />
      <component name="UnderlyingInstrument" required="N" />
      <field name="Side" required="N" />
      <field name="TransactTime" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="OrderMassStatusRequest" msgtype="AF" msgcat="app">
      <field name="MassStatusReqID" required="Y" />
      <field name="MassStatusReqType" required="Y" />
      <component name="Parties" required="N" />
      <field name="Account" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <component name="Instrument" required="N" />
      <component name="UnderlyingInstrument" required="N" />
      <field name="Side" required="N" />
    </message>
    <message name="NewOrderCross" msgtype="s" msgcat="app">
      <field name="CrossID" required="Y" />
      <field name="CrossType" required="Y" />
      <field name="CrossPrioritization" required="Y" />
      <group name="NoSides" required="Y">
        <field name="Side" required="Y" />
        <field name="ClOrdID" required="Y" />
        <field name="SecondaryClOrdID" required="N" />
        <field name="ClOrdLinkID" required="N" />
        <component name="Parties" required="N" />
        <field name="TradeOriginationDate" required="N" />
        <field name="Account" required="N" />
        <field name="AccountType" required="N" />
        <field name="DayBookingInst" required="N" />
        <field name="BookingUnit" required="N" />
        <field name="PreallocMethod" required="N" />
        <group name="NoAllocs" required="N">
          <field name="AllocAccount" required="N" />
          <field name="IndividualAllocID" required="N" />
          <component name="NestedParties" required="N" />
          <field name="AllocQty" required="N" />
        </group>
        <field name="QuantityType" required="N" />
        <component name="OrderQtyData" required="Y" />
        <component name="CommissionData" required="N" />
        <field name="OrderCapacity" required="N" />
        <field name="OrderRestrictions" required="N" />
        <field name="CustOrderCapacity" required="N" />
        <field name="ForexReq" required="N" />
        <field name="SettlCurrency" required="N" />
        <field name="Text" required="N" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
        <field name="PositionEffect" required="N" />
        <field name="CoveredOrUncovered" required="N" />
        <field name="CashMargin" required="N" />
        <field name="ClearingFeeIndicator" required="N" />
        <field name="SolicitedFlag" required="N" />
        <field name="SideComplianceID" required="N" />
      </group>
      <component name="Instrument" required="Y" />
      <field name="SettlmntTyp" required="N" />
      <field name="FutSettDate" required="N" />
      <field name="HandlInst" required="Y" />
      <field name="ExecInst" required="N" />
      <field name="MinQty" required="N" />
      <field name="MaxFloor" required="N" />
      <field name="ExDestination" required="N" />
      <group name="NoTradingSessions" required="N">
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
      </group>
      <field name="ProcessCode" required="N" />
      <field name="PrevClosePx" required="N" />
      <field name="LocateReqd" required="N" />
      <field name="TransactTime" required="Y" />
      <component name="Stipulations" required="N" />
      <field name="OrdType" required="Y" />
      <field name="PriceType" required="N" />
      <field name="Price" required="N" />
      <field name="StopPx" required="N" />
      <component name="SpreadOrBenchmarkCurveData" required="N" />
      <component name="YieldData" required="N" />
      <field name="Currency" required="N" />
      <field name="ComplianceID" required="N" />
      <field name="IOIid" required="N" />
      <field name="QuoteID" required="N" />
      <field name="TimeInForce" required="N" />
      <field name="EffectiveTime" required="N" />
      <field name="ExpireDate" required="N" />
      <field name="ExpireTime" required="N" />
      <field name="GTBookingInst" required="N" />
      <field name="MaxShow" required="N" />
      <field name="PegDifference" required="N" />
      <field name="DiscretionInst" required="N" />
      <field name="DiscretionOffset" required="N" />
      <field name="CancellationRights" required="N" />
      <field name="MoneyLaunderingStatus" required="N" />
      <field name="RegistID" required="N" />
      <field name="Designation" required="N" />
      <field name="AccruedInterestRate" required="N" />
      <field name="AccruedInterestAmt" required="N" />
      <field name="NetMoney" required="N" />
    </message>
    <message name="CrossOrderCancelReplaceRequest" msgtype="t" msgcat="app">
      <field name="OrderID" required="N" />
      <field name="CrossID" required="Y" />
      <field name="OrigCrossID" required="Y" />
      <field name="CrossType" required="Y" />
      <field name="CrossPrioritization" required="Y" />
      <group name="NoSides" required="Y">
        <field name="Side" required="Y" />
        <field name="OrigClOrdID" required="N" />
        <field name="ClOrdID" required="Y" />
        <field name="SecondaryClOrdID" required="N" />
        <field name="ClOrdLinkID" required="N" />
        <field name="OrigOrdModTime" required="N" />
        <component name="Parties" required="N" />
        <field name="TradeOriginationDate" required="N" />
        <field name="Account" required="N" />
        <field name="AccountType" required="N" />
        <field name="DayBookingInst" required="N" />
        <field name="BookingUnit" required="N" />
        <field name="PreallocMethod" required="N" />
        <group name="NoAllocs" required="N">
          <field name="AllocAccount" required="N" />
          <field name="IndividualAllocID" required="N" />
          <component name="NestedParties" required="N" />
          <field name="AllocQty" required="N" />
        </group>
        <field name="QuantityType" required="N" />
        <component name="OrderQtyData" required="Y" />
        <component name="CommissionData" required="N" />
        <field name="OrderCapacity" required="N" />
        <field name="OrderRestrictions" required="N" />
        <field name="CustOrderCapacity" required="N" />
        <field name="ForexReq" required="N" />
        <field name="SettlCurrency" required="N" />
        <field name="Text" required="N" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
        <field name="PositionEffect" required="N" />
        <field name="CoveredOrUncovered" required="N" />
        <field name="CashMargin" required="N" />
        <field name="ClearingFeeIndicator" required="N" />
        <field name="SolicitedFlag" required="N" />
        <field name="SideComplianceID" required="N" />
      </group>
      <component name="Instrument" required="Y" />
      <field name="SettlmntTyp" required="N" />
      <field name="FutSettDate" required="N" />
      <field name="HandlInst" required="Y" />
      <field name="ExecInst" required="N" />
      <field name="MinQty" required="N" />
      <field name="MaxFloor" required="N" />
      <field name="ExDestination" required="N" />
      <group name="NoTradingSessions" required="N">
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
      </group>
      <field name="ProcessCode" required="N" />
      <field name="PrevClosePx" required="N" />
      <field name="LocateReqd" required="N" />
      <field name="TransactTime" required="Y" />
      <component name="Stipulations" required="N" />
      <field name="OrdType" required="Y" />
      <field name="PriceType" required="N" />
      <field name="Price" required="N" />
      <field name="StopPx" required="N" />
      <component name="SpreadOrBenchmarkCurveData" required="N" />
      <component name="YieldData" required="N" />
      <field name="Currency" required="N" />
      <field name="ComplianceID" required="N" />
      <field name="IOIid" required="N" />
      <field name="QuoteID" required="N" />
      <field name="TimeInForce" required="N" />
      <field name="EffectiveTime" required="N" />
      <field name="ExpireDate" required="N" />
      <field name="ExpireTime" required="N" />
      <field name="GTBookingInst" required="N" />
      <field name="MaxShow" required="N" />
      <field name="PegDifference" required="N" />
      <field name="DiscretionInst" required="N" />
      <field name="DiscretionOffset" required="N" />
      <field name="CancellationRights" required="N" />
      <field name="MoneyLaunderingStatus" required="N" />
      <field name="RegistID" required="N" />
      <field name="Designation" required="N" />
      <field name="AccruedInterestRate" required="N" />
      <field name="AccruedInterestAmt" required="N" />
      <field name="NetMoney" required="N" />
    </message>
    <message name="CrossOrderCancelRequest" msgtype="u" msgcat="app">
      <field name="OrderID" required="N" />
      <field name="CrossID" required="Y" />
      <field name="OrigCrossID" required="Y" />
      <field name="CrossType" required="Y" />
      <field name="CrossPrioritization" required="Y" />
      <group name="NoSides" required="Y">
        <field name="Side" required="Y" />
        <field name="OrigClOrdID" required="N" />
        <field name="ClOrdID" required="Y" />
        <field name="SecondaryClOrdID" required="N" />
        <field name="ClOrdLinkID" required="N" />
        <field name="OrigOrdModTime" required="N" />
        <component name="Parties" required="N" />
        <field name="TradeOriginationDate" required="N" />
        <component name="OrderQtyData" required="Y" />
        <field name="ComplianceID" required="N" />
        <field name="Text" required="N" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
      </group>
      <component name="Instrument" required="Y" />
      <field name="TransactTime" required="Y" />
    </message>
    <message name="NewOrderMultileg" msgtype="AB" msgcat="app">
      <field name="ClOrdID" required="Y" />
      <field name="SecondaryClOrdID" required="N" />
      <field name="ClOrdLinkID" required="N" />
      <component name="Parties" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <field name="DayBookingInst" required="N" />
      <field name="BookingUnit" required="N" />
      <field name="PreallocMethod" required="N" />
      <group name="NoAllocs" required="N">
        <field name="AllocAccount" required="N" />
        <field name="IndividualAllocID" required="N" />
        <field name="AllocQty" required="N" />
      </group>
      <field name="SettlmntTyp" required="N" />
      <field name="FutSettDate" required="N" />
      <field name="CashMargin" required="N" />
      <field name="ClearingFeeIndicator" required="N" />
      <field name="HandlInst" required="Y" />
      <field name="ExecInst" required="N" />
      <field name="MinQty" required="N" />
      <field name="MaxFloor" required="N" />
      <field name="ExDestination" required="N" />
      <group name="NoTradingSessions" required="N">
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
      </group>
      <field name="ProcessCode" required="N" />
      <field name="Side" required="Y" />
      <component name="Instrument" required="Y" />
      <field name="PrevClosePx" required="N" />
      <group name="NoLegs" required="N">
        <component name="InstrumentLeg" required="N" />
        <field name="LegPositionEffect" required="N" />
        <field name="LegCoveredOrUncovered" required="N" />
        <component name="NestedParties" required="N" />
        <field name="LegRefID" required="N" />
        <field name="LegPrice" required="N" />
        <field name="LegSettlmntTyp" required="N" />
        <field name="LegFutSettDate" required="N" />
      </group>
      <field name="LocateReqd" required="N" />
      <field name="TransactTime" required="Y" />
      <field name="QuantityType" required="N" />
      <component name="OrderQtyData" required="Y" />
      <field name="OrdType" required="Y" />
      <field name="PriceType" required="N" />
      <field name="Price" required="N" />
      <field name="StopPx" required="N" />
      <field name="Currency" required="N" />
      <field name="ComplianceID" required="N" />
      <field name="SolicitedFlag" required="N" />
      <field name="IOIid" required="N" />
      <field name="QuoteID" required="N" />
      <field name="TimeInForce" required="N" />
      <field name="EffectiveTime" required="N" />
      <field name="ExpireDate" required="N" />
      <field name="ExpireTime" required="N" />
      <field name="GTBookingInst" required="N" />
      <component name="CommissionData" required="N" />
      <field name="OrderCapacity" required="N" />
      <field name="OrderRestrictions" required="N" />
      <field name="CustOrderCapacity" required="N" />
      <field name="ForexReq" required="N" />
      <field name="SettlCurrency" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="PositionEffect" required="N" />
      <field name="CoveredOrUncovered" required="N" />
      <field name="MaxShow" required="N" />
      <field name="PegDifference" required="N" />
      <field name="DiscretionInst" required="N" />
      <field name="DiscretionOffset" required="N" />
      <field name="CancellationRights" required="N" />
      <field name="MoneyLaunderingStatus" required="N" />
      <field name="RegistID" required="N" />
      <field name="Designation" required="N" />
      <field name="MultiLegRptTypeReq" required="N" />
      <field name="NetMoney" required="N" />
    </message>
    <message name="MultilegOrderCancelReplaceRequest" msgtype="AC" msgcat="app">
      <field name="OrderID" required="N" />
      <field name="OrigClOrdID" required="Y" />
      <field name="ClOrdID" required="Y" />
      <field name="SecondaryClOrdID" required="N" />
      <field name="ClOrdLinkID" required="N" />
      <field name="OrigOrdModTime" required="N" />
      <component name="Parties" required="N" />
      <field name="Account" required="N" />
      <field name="AccountType" required="N" />
      <field name="DayBookingInst" required="N" />
      <field name="BookingUnit" required="N" />
      <field name="PreallocMethod" required="N" />
      <group name="NoAllocs" required="N">
        <field name="AllocAccount" required="N" />
        <field name="IndividualAllocID" required="N" />
        <field name="AllocQty" required="N" />
      </group>
      <field name="SettlmntTyp" required="N" />
      <field name="FutSettDate" required="N" />
      <field name="CashMargin" required="N" />
      <field name="ClearingFeeIndicator" required="N" />
      <field name="HandlInst" required="Y" />
      <field name="ExecInst" required="N" />
      <field name="MinQty" required="N" />
      <field name="MaxFloor" required="N" />
      <field name="ExDestination" required="N" />
      <group name="NoTradingSessions" required="N">
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
      </group>
      <field name="ProcessCode" required="N" />
      <field name="Side" required="Y" />
      <component name="Instrument" required="Y" />
      <field name="PrevClosePx" required="N" />
      <group name="NoLegs" required="N">
        <component name="InstrumentLeg" required="N" />
        <field name="LegPositionEffect" required="N" />
        <field name="LegCoveredOrUncovered" required="N" />
        <component name="NestedParties" required="N" />
        <field name="LegRefID" required="N" />
        <field name="LegPrice" required="N" />
        <field name="LegSettlmntTyp" required="N" />
        <field name="LegFutSettDate" required="N" />
      </group>
      <field name="LocateReqd" required="N" />
      <field name="TransactTime" required="Y" />
      <field name="QuantityType" required="N" />
      <component name="OrderQtyData" required="Y" />
      <field name="OrdType" required="Y" />
      <field name="PriceType" required="N" />
      <field name="Price" required="N" />
      <field name="StopPx" required="N" />
      <field name="Currency" required="N" />
      <field name="ComplianceID" required="N" />
      <field name="SolicitedFlag" required="N" />
      <field name="IOIid" required="N" />
      <field name="QuoteID" required="N" />
      <field name="TimeInForce" required="N" />
      <field name="EffectiveTime" required="N" />
      <field name="ExpireDate" required="N" />
      <field name="ExpireTime" required="N" />
      <field name="GTBookingInst" required="N" />
      <component name="CommissionData" required="N" />
      <field name="OrderCapacity" required="N" />
      <field name="OrderRestrictions" required="N" />
      <field name="CustOrderCapacity" required="N" />
      <field name="ForexReq" required="N" />
      <field name="SettlCurrency" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="PositionEffect" required="N" />
      <field name="CoveredOrUncovered" required="N" />
      <field name="MaxShow" required="N" />
      <field name="PegDifference" required="N" />
      <field name="DiscretionInst" required="N" />
      <field name="DiscretionOffset" required="N" />
      <field name="CancellationRights" required="N" />
      <field name="MoneyLaunderingStatus" required="N" />
      <field name="RegistID" required="N" />
      <field name="Designation" required="N" />
      <field name="MultiLegRptTypeReq" required="N" />
      <field name="NetMoney" required="N" />
    </message>
    <message name="BidRequest" msgtype="k" msgcat="app">
      <field name="BidID" required="N" />
      <field name="ClientBidID" required="Y" />
      <field name="BidRequestTransType" required="Y" />
      <field name="ListName" required="N" />
      <field name="TotalNumSecurities" required="Y" />
      <field name="BidType" required="Y" />
      <field name="NumTickets" required="N" />
      <field name="Currency" required="N" />
      <field name="SideValue1" required="N" />
      <field name="SideValue2" required="N" />
      <group name="NoBidDescriptors" required="N">
        <field name="BidDescriptorType" required="N" />
        <field name="BidDescriptor" required="N" />
        <field name="SideValueInd" required="N" />
        <field name="LiquidityValue" required="N" />
        <field name="LiquidityNumSecurities" required="N" />
        <field name="LiquidityPctLow" required="N" />
        <field name="LiquidityPctHigh" required="N" />
        <field name="EFPTrackingError" required="N" />
        <field name="FairValue" required="N" />
        <field name="OutsideIndexPct" required="N" />
        <field name="ValueOfFutures" required="N" />
      </group>
      <group name="NoBidComponents" required="N">
        <field name="ListID" required="N" />
        <field name="Side" required="N" />
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
        <field name="NetGrossInd" required="N" />
        <field name="SettlmntTyp" required="N" />
        <field name="FutSettDate" required="N" />
        <field name="Account" required="N" />
      </group>
      <field name="LiquidityIndType" required="N" />
      <field name="WtAverageLiquidity" required="N" />
      <field name="ExchangeForPhysical" required="N" />
      <field name="OutMainCntryUIndex" required="N" />
      <field name="CrossPercent" required="N" />
      <field name="ProgRptReqs" required="N" />
      <field name="ProgPeriodInterval" required="N" />
      <field name="IncTaxInd" required="N" />
      <field name="ForexReq" required="N" />
      <field name="NumBidders" required="N" />
      <field name="TradeDate" required="N" />
      <field name="TradeType" required="Y" />
      <field name="BasisPxType" required="Y" />
      <field name="StrikeTime" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="BidResponse" msgtype="l" msgcat="app">
      <field name="BidID" required="N" />
      <field name="ClientBidID" required="N" />
      <group name="NoBidComponents" required="Y">
        <component name="CommissionData" required="Y" />
        <field name="ListID" required="N" />
        <field name="Country" required="N" />
        <field name="Side" required="N" />
        <field name="Price" required="N" />
        <field name="PriceType" required="N" />
        <field name="FairValue" required="N" />
        <field name="NetGrossInd" required="N" />
        <field name="SettlmntTyp" required="N" />
        <field name="FutSettDate" required="N" />
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
        <field name="Text" required="N" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
      </group>
    </message>
    <message name="NewOrderList" msgtype="E" msgcat="app">
      <field name="ListID" required="Y" />
      <field name="BidID" required="N" />
      <field name="ClientBidID" required="N" />
      <field name="ProgRptReqs" required="N" />
      <field name="BidType" required="Y" />
      <field name="ProgPeriodInterval" required="N" />
      <field name="CancellationRights" required="N" />
      <field name="MoneyLaunderingStatus" required="N" />
      <field name="RegistID" required="N" />
      <field name="ListExecInstType" required="N" />
      <field name="ListExecInst" required="N" />
      <field name="EncodedListExecInstLen" required="N" />
      <field name="EncodedListExecInst" required="N" />
      <field name="TotNoOrders" required="Y" />
      <group name="NoOrders" required="Y">
        <field name="ClOrdID" required="Y" />
        <field name="SecondaryClOrdID" required="N" />
        <field name="ListSeqNo" required="Y" />
        <field name="ClOrdLinkID" required="N" />
        <field name="SettlInstMode" required="N" />
        <component name="Parties" required="N" />
        <field name="TradeOriginationDate" required="N" />
        <field name="Account" required="N" />
        <field name="AccountType" required="N" />
        <field name="DayBookingInst" required="N" />
        <field name="BookingUnit" required="N" />
        <field name="PreallocMethod" required="N" />
        <group name="NoAllocs" required="N">
          <field name="AllocAccount" required="N" />
          <field name="IndividualAllocID" required="N" />
          <component name="NestedParties" required="N" />
          <field name="AllocQty" required="N" />
        </group>
        <field name="SettlmntTyp" required="N" />
        <field name="FutSettDate" required="N" />
        <field name="CashMargin" required="N" />
        <field name="ClearingFeeIndicator" required="N" />
        <field name="HandlInst" required="N" />
        <field name="ExecInst" required="N" />
        <field name="MinQty" required="N" />
        <field name="MaxFloor" required="N" />
        <field name="ExDestination" required="N" />
        <group name="NoTradingSessions" required="N">
          <field name="TradingSessionID" required="N" />
          <field name="TradingSessionSubID" required="N" />
        </group>
        <field name="ProcessCode" required="N" />
        <component name="Instrument" required="Y" />
        <field name="PrevClosePx" required="N" />
        <field name="Side" required="Y" />
        <field name="SideValueInd" required="N" />
        <field name="LocateReqd" required="N" />
        <field name="TransactTime" required="N" />
        <component name="Stipulations" required="N" />
        <field name="QuantityType" required="N" />
        <component name="OrderQtyData" required="Y" />
        <field name="OrdType" required="N" />
        <field name="PriceType" required="N" />
        <field name="Price" required="N" />
        <field name="StopPx" required="N" />
        <component name="SpreadOrBenchmarkCurveData" required="N" />
        <component name="YieldData" required="N" />
        <field name="Currency" required="N" />
        <field name="ComplianceID" required="N" />
        <field name="SolicitedFlag" required="N" />
        <field name="IOIid" required="N" />
        <field name="QuoteID" required="N" />
        <field name="TimeInForce" required="N" />
        <field name="EffectiveTime" required="N" />
        <field name="ExpireDate" required="N" />
        <field name="ExpireTime" required="N" />
        <field name="GTBookingInst" required="N" />
        <component name="CommissionData" required="N" />
        <field name="OrderCapacity" required="N" />
        <field name="OrderRestrictions" required="N" />
        <field name="CustOrderCapacity" required="N" />
        <field name="Rule80A" required="N" />
        <field name="ForexReq" required="N" />
        <field name="SettlCurrency" required="N" />
        <field name="Text" required="N" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
        <field name="FutSettDate2" required="N" />
        <field name="OrderQty2" required="N" />
        <field name="Price2" required="N" />
        <field name="PositionEffect" required="N" />
        <field name="CoveredOrUncovered" required="N" />
        <field name="MaxShow" required="N" />
        <field name="PegDifference" required="N" />
        <field name="DiscretionInst" required="N" />
        <field name="DiscretionOffset" required="N" />
        <field name="Designation" required="N" />
        <field name="AccruedInterestRate" required="N" />
        <field name="AccruedInterestAmt" required="N" />
        <field name="NetMoney" required="N" />
      </group>
    </message>
    <message name="ListStrikePrice" msgtype="m" msgcat="app">
      <field name="ListID" required="Y" />
      <field name="TotNoStrikes" required="Y" />
      <group name="NoStrikes" required="Y">
        <component name="Instrument" required="Y" />
        <field name="PrevClosePx" required="N" />
        <field name="ClOrdID" required="N" />
        <field name="SecondaryClOrdID" required="N" />
        <field name="Side" required="N" />
        <field name="Price" required="Y" />
        <field name="Currency" required="N" />
        <field name="Text" required="N" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
      </group>
    </message>
    <message name="ListExecute" msgtype="L" msgcat="app">
      <field name="ListID" required="Y" />
      <field name="ClientBidID" required="N" />
      <field name="BidID" required="N" />
      <field name="TransactTime" required="Y" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="ListCancelRequest" msgtype="K" msgcat="app">
      <field name="ListID" required="Y" />
      <field name="TransactTime" required="Y" />
      <field name="TradeOriginationDate" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="ListStatusRequest" msgtype="M" msgcat="app">
      <field name="ListID" required="Y" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
    </message>
    <message name="ListStatus" msgtype="N" msgcat="app">
      <field name="ListID" required="Y" />
      <field name="ListStatusType" required="Y" />
      <field name="NoRpts" required="Y" />
      <field name="ListOrderStatus" required="Y" />
      <field name="RptSeq" required="Y" />
      <field name="ListStatusText" required="N" />
      <field name="EncodedListStatusTextLen" required="N" />
      <field name="EncodedListStatusText" required="N" />
      <field name="TransactTime" required="N" />
      <field name="TotNoOrders" required="Y" />
      <group name="NoOrders" required="Y">
        <field name="ClOrdID" required="Y" />
        <field name="SecondaryClOrdID" required="N" />
        <field name="CumQty" required="Y" />
        <field name="OrdStatus" required="Y" />
        <field name="WorkingIndicator" required="N" />
        <field name="LeavesQty" required="Y" />
        <field name="CxlQty" required="Y" />
        <field name="AvgPx" required="Y" />
        <field name="OrdRejReason" required="N" />
        <field name="Text" required="N" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
      </group>
    </message>
    <message name="Allocation" msgtype="J" msgcat="app">
      <field name="AllocID" required="Y" />
      <field name="AllocTransType" required="Y" />
      <field name="AllocType" required="Y" />
      <field name="RefAllocID" required="N" />
      <field name="AllocLinkID" required="N" />
      <field name="AllocLinkType" required="N" />
      <field name="BookingRefID" required="N" />
      <group name="NoOrders" required="Y">
        <field name="ClOrdID" required="Y" />
        <field name="OrderID" required="N" />
        <field name="SecondaryOrderID" required="N" />
        <field name="SecondaryClOrdID" required="N" />
        <field name="ListID" required="N" />
      </group>
      <group name="NoExecs" required="N">
        <field name="LastQty" required="N" />      
        <field name="ExecID" required="N" />
        <field name="SecondaryExecID" required="N" />
        <field name="LastPx" required="N" />
        <field name="LastCapacity" required="N" />
      </group>
      <field name="Side" required="Y" />
      <component name="Instrument" required="Y" />
      <field name="Quantity" required="Y" />
      <field name="LastMkt" required="N" />
      <field name="TradeOriginationDate" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <field name="PriceType" required="N" />
      <field name="AvgPx" required="Y" />
      <field name="Currency" required="N" />
      <field name="AvgPrxPrecision" required="N" />
      <component name="Parties" required="N" />
      <field name="TradeDate" required="Y" />
      <field name="TransactTime" required="N" />
      <field name="SettlmntTyp" required="N" />
      <field name="FutSettDate" required="N" />
      <field name="GrossTradeAmt" required="N" />
      <field name="Concession" required="N" />
      <field name="TotalTakedown" required="N" />
      <field name="NetMoney" required="N" />
      <field name="PositionEffect" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="NumDaysInterest" required="N" />
      <field name="AccruedInterestRate" required="N" />
      <field name="TotalAccruedInterestAmt" required="N" />
      <field name="LegalConfirm" required="N" />
      <group name="NoAllocs" required="Y">
        <field name="AllocAccount" required="Y" />
        <field name="AllocPrice" required="N" />
        <field name="AllocQty" required="Y" />
        <field name="IndividualAllocID" required="N" />
        <field name="ProcessCode" required="N" />
        <component name="NestedParties" required="N" />
        <field name="NotifyBrokerOfCredit" required="N" />
        <field name="AllocHandlInst" required="N" />
        <field name="AllocText" required="N" />
        <field name="EncodedAllocTextLen" required="N" />
        <field name="EncodedAllocText" required="N" />
        <component name="CommissionData" required="N" />
        <field name="AllocAvgPx" required="N" />
        <field name="AllocNetMoney" required="N" />
        <field name="SettlCurrAmt" required="N" />
        <field name="SettlCurrency" required="N" />
        <field name="SettlCurrFxRate" required="N" />
        <field name="SettlCurrFxRateCalc" required="N" />
        <field name="AccruedInterestAmt" required="N" />
        <field name="SettlInstMode" required="N" />
        <group name="NoMiscFees" required="N">
          <field name="MiscFeeAmt" required="N" />
          <field name="MiscFeeCurr" required="N" />
          <field name="MiscFeeType" required="N" />
        </group>
      </group>
    </message>
    <message name="AllocationACK" msgtype="P" msgcat="app">
      <component name="Parties" required="N" />
      <field name="AllocID" required="Y" />
      <field name="TradeDate" required="Y" />
      <field name="TransactTime" required="N" />
      <field name="AllocStatus" required="Y" />
      <field name="AllocRejCode" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="LegalConfirm" required="N" />
    </message>
    <message name="SettlementInstructions" msgtype="T" msgcat="app">
      <field name="SettlInstID" required="Y" />
      <field name="SettlInstTransType" required="Y" />
      <field name="SettlInstRefID" required="Y" />
      <field name="SettlInstMode" required="Y" />
      <field name="SettlInstSource" required="Y" />
      <field name="AllocAccount" required="Y" />
      <field name="IndividualAllocID" required="N" />
      <field name="ClOrdID" required="N" />
      <field name="TradeDate" required="N" />
      <field name="AllocID" required="N" />
      <field name="LastMkt" required="N" />
      <field name="TradingSessionID" required="N" />
      <field name="TradingSessionSubID" required="N" />
      <field name="Side" required="N" />
      <field name="SecurityType" required="N" />
      <field name="EffectiveTime" required="N" />
      <field name="TransactTime" required="Y" />
      <component name="Parties" required="N" />
      <field name="StandInstDbType" required="N" />
      <field name="StandInstDbName" required="N" />
      <field name="StandInstDbID" required="N" />
      <field name="SettlDeliveryType" required="N" />
      <field name="SettlDepositoryCode" required="N" />
      <field name="SettlBrkrCode" required="N" />
      <field name="SettlInstCode" required="N" />
      <field name="SecuritySettlAgentName" required="N" />
      <field name="SecuritySettlAgentCode" required="N" />
      <field name="SecuritySettlAgentAcctNum" required="N" />
      <field name="SecuritySettlAgentAcctName" required="N" />
      <field name="SecuritySettlAgentContactName" required="N" />
      <field name="SecuritySettlAgentContactPhone" required="N" />
      <field name="CashSettlAgentName" required="N" />
      <field name="CashSettlAgentCode" required="N" />
      <field name="CashSettlAgentAcctNum" required="N" />
      <field name="CashSettlAgentAcctName" required="N" />
      <field name="CashSettlAgentContactName" required="N" />
      <field name="CashSettlAgentContactPhone" required="N" />
      <field name="PaymentMethod" required="N" />
      <field name="PaymentRef" required="N" />
      <field name="CardHolderName" required="N" />
      <field name="CardNumber" required="N" />
      <field name="CardStartDate" required="N" />
      <field name="CardExpDate" required="N" />
      <field name="CardIssNo" required="N" />
      <field name="PaymentDate" required="N" />
      <field name="PaymentRemitterID" required="N" />
    </message>
    <message name="TradeCaptureReportRequest" msgtype="AD" msgcat="app">
      <field name="TradeRequestID" required="Y" />
      <field name="TradeRequestType" required="Y" />
      <field name="SubscriptionRequestType" required="N" />
      <field name="ExecID" required="N" />
      <field name="OrderID" required="N" />
      <field name="ClOrdID" required="N" />
      <field name="MatchStatus" required="N" />
      <component name="Parties" required="N" />
      <component name="Instrument" required="N" />
      <group name="NoDates" required="N">
        <field name="TradeDate" required="N" />
        <field name="TransactTime" required="N" />
      </group>
      <field name="Side" required="N" />
      <field name="Text" required="N" />
      <field name="EncodedTextLen" required="N" />
      <field name="EncodedText" required="N" />
      <field name="TradeInputSource" required="N" />
      <field name="TradeInputDevice" required="N" />
    </message>
    <message name="TradeCaptureReport" msgtype="AE" msgcat="app">
      <field name="TradeReportID" required="Y" />
      <field name="TradeReportTransType" required="N" />
      <field name="TradeRequestID" required="N" />
      <field name="ExecType" required="N" />
      <field name="TradeReportRefID" required="N" />
      <field name="ExecID" required="N" />
      <field name="SecondaryExecID" required="N" />
      <field name="ExecRestatementReason" required="N" />
      <field name="PreviouslyReported" required="N" />
      <component name="Instrument" required="Y" />
      <component name="OrderQtyData" required="N" />
      <field name="LastQty" required="Y" />
      <field name="OrderQty2" required="N" />
      <field name="LastPx" required="Y" />
	  <field name="LastPx2" required="N" />
      <field name="LastSpotRate" required="N" />
      <field name="LastForwardPoints" required="N" />
	  <field name="LastForwardPoints2" required="N" />
      <field name="LastMkt" required="N" />
      <field name="TradeDate" required="Y" />
      <field name="TransactTime" required="Y" />
      <field name="SettlmntTyp" required="N" />
      <field name="FutSettDate" required="N" />
	  <field name="FutSettDate2" required="N" />
      <field name="MatchStatus" required="N" />
      <field name="MatchType" required="N" />
	  <field name="Currency" required="N" />
      
	  <field name="Price" required="Y" />
	  <field name="Price2" required="N" />
	  <field name="SettlCurrAmt" required="Y" />
	  <field name="SettlCurrAmt2" required="N" />
	  <field name="SettlCurrency" required="Y" />
	  <field name="SecurityType" required="N" />
	  <field name="CoverTxnIds" required="N" />
	  <field name="CoveredTxnIds" required="N" />
      <group name="NoSides" required="N">
        <field name="Side" required="Y" />
        <field name="OrderID" required="N" />
        <field name="SecondaryOrderID" required="N" />
        <field name="ClOrdID" required="N" />
        <component name="Parties" required="N" />
        <field name="Account" required="N" />
        <field name="AccountType" required="N" />
        <field name="ProcessCode" required="N" />
        <field name="OddLot" required="N" />
        <group name="NoClearingInstructions" required="N">
          <field name="ClearingInstruction" required="N" />
        </group>
        <field name="ClearingFeeIndicator" required="N" />
        <field name="TradeInputSource" required="N" />
        <field name="TradeInputDevice" required="N" />
        <field name="ComplianceID" required="N" />
        <field name="SolicitedFlag" required="N" />
        <field name="OrderCapacity" required="N" />
        <field name="OrderRestrictions" required="N" />
        <field name="CustOrderCapacity" required="N" />
        <field name="TransBkdTime" required="N" />
        <field name="TradingSessionID" required="N" />
        <field name="TradingSessionSubID" required="N" />
        <component name="CommissionData" required="N" />
        <field name="GrossTradeAmt" required="N" />
        <field name="NumDaysInterest" required="N" />
        <field name="ExDate" required="N" />
        <field name="AccruedInterestRate" required="N" />
        <field name="AccruedInterestAmt" required="N" />
        <field name="Concession" required="N" />
        <field name="TotalTakedown" required="N" />
        <field name="NetMoney" required="N" />
        <field name="SettlCurrFxRate" required="N" />
        <field name="SettlCurrFxRateCalc" required="N" />
        <field name="PositionEffect" required="N" />
        <field name="Text" required="N" />
        <field name="EncodedTextLen" required="N" />
        <field name="EncodedText" required="N" />
        <field name="MultiLegReportingType" required="N" />
        <group name="NoContAmts" required="N">
          <field name="ContAmtType" required="N" />
          <field name="ContAmtValue" required="N" />
          <field name="ContAmtCurr" required="N" />        
        </group>
        <group name="NoMiscFees" required="N">
          <field name="MiscFeeAmt" required="N" />
          <field name="MiscFeeCurr" required="N" />
          <field name="MiscFeeType" required="N" />
        </group>
      </group>
    </message>
    <message name="RegistrationInstructions" msgtype="o" msgcat="app">
      <field name="RegistID" required="Y" />
      <field name="RegistTransType" required="Y" />
      <field name="RegistRefID" required="Y" />
      <field name="ClOrdID" required="N" />
      <component name="Parties" required="N" />
      <field name="Account" required="N" />
      <field name="RegistAcctType" required="N" />
      <field name="TaxAdvantageType" required="N" />
      <field name="OwnershipType" required="N" />
      <group name="NoRegistDtls" required="N">
        <field name="RegistDetls" required="N" />
        <field name="RegistEmail" required="N" />
        <field name="MailingDtls" required="N" />
        <field name="MailingInst" required="N" />
        <component name="NestedParties" required="N" />
        <field name="OwnerType" required="N" />
        <field name="DateOfBirth" required="N" />
        <field name="InvestorCountryOfResidence" required="N" />
      </group>
      <group name="NoDistribInsts" required="N">
        <field name="DistribPaymentMethod" required="N" />
        <field name="DistribPercentage" required="N" />
        <field name="CashDistribCurr" required="N" />
        <field name="CashDistribAgentName" required="N" />
        <field name="CashDistribAgentCode" required="N" />
        <field name="CashDistribAgentAcctNumber" required="N" />
        <field name="CashDistribPayRef" required="N" />
        <field name="CashDistribAgentAcctName" required="N" />
      </group>
    </message>
    <message name="RegistrationInstructionsResponse" msgtype="p" msgcat="app">
      <field name="RegistID" required="Y" />
      <field name="RegistTransType" required="Y" />
      <field name="RegistRefID" required="Y" />
      <field name="ClOrdID" required="N" />
      <component name="Parties" required="N" />
      <field name="Account" required="N" />
      <field name="RegistStatus" required="Y" />
      <field name="RegistRejReasonCode" required="N" />
      <field name="RegistRejReasonText" required="N" />
    </message>
  </messages>
  <components>
    <component name="Instrument">
      <field name="Symbol" required="Y" />
      <field name="SymbolSfx" required="N" />
      <field name="SecurityID" required="N" />
      <field name="SecurityIDSource" required="N" />
      <group name="NoSecurityAltID" required="N">
        <field name="SecurityAltID" required="N" />
        <field name="SecurityAltIDSource" required="N" />
      </group>
      <field name="Product" required="N" />
      <field name="CFICode" required="N" />
      <field name="SecurityType" required="N" />
      <field name="MaturityMonthYear" required="N" />
      <field name="MaturityDate" required="N" />
      <field name="CouponPaymentDate" required="N" />
      <field name="IssueDate" required="N" />
      <field name="RepoCollateralSecurityType" required="N" />
      <field name="RepurchaseTerm" required="N" />
      <field name="RepurchaseRate" required="N" />
      <field name="Factor" required="N" />
      <field name="CreditRating" required="N" />
      <field name="InstrRegistry" required="N" />
      <field name="CountryOfIssue" required="N" />
      <field name="StateOrProvinceOfIssue" required="N" />
      <field name="LocaleOfIssue" required="N" />
      <field name="RedemptionDate" required="N" />
      <field name="StrikePrice" required="N" />
      <field name="OptAttribute" required="N" />
      <field name="ContractMultiplier" required="N" />
      <field name="CouponRate" required="N" />
      <field name="SecurityExchange" required="N" />
      <field name="Issuer" required="N" />
      <field name="EncodedIssuerLen" required="N" />
      <field name="EncodedIssuer" required="N" />
      <field name="SecurityDesc" required="N" />
      <field name="EncodedSecurityDescLen" required="N" />
      <field name="EncodedSecurityDesc" required="N" />
    </component>
    <component name="InstrumentLeg">
      <field name="LegSymbol" required="Y" />
      <field name="LegSymbolSfx" required="N" />
      <field name="LegSecurityID" required="N" />
      <field name="LegSecurityIDSource" required="N" />
      <group name="NoLegSecurityAltID" required="N">
        <field name="LegSecurityAltID" required="N" />
        <field name="LegSecurityAltIDSource" required="N" />
      </group>
      <field name="LegProduct" required="N" />
      <field name="LegCFICode" required="N" />
      <field name="LegSecurityType" required="N" />
      <field name="LegMaturityMonthYear" required="N" />
      <field name="LegMaturityDate" required="N" />
      <field name="LegCouponPaymentDate" required="N" />
      <field name="LegIssueDate" required="N" />
      <field name="LegRepoCollateralSecurityType" required="N" />
      <field name="LegRepurchaseTerm" required="N" />
      <field name="LegRepurchaseRate" required="N" />
      <field name="LegFactor" required="N" />
      <field name="LegCreditRating" required="N" />
      <field name="LegInstrRegistry" required="N" />
      <field name="LegCountryOfIssue" required="N" />
      <field name="LegStateOrProvinceOfIssue" required="N" />
      <field name="LegLocaleOfIssue" required="N" />
      <field name="LegRedemptionDate" required="N" />
      <field name="LegStrikePrice" required="N" />
      <field name="LegOptAttribute" required="N" />
      <field name="LegContractMultiplier" required="N" />
      <field name="LegCouponRate" required="N" />
      <field name="LegSecurityExchange" required="N" />
      <field name="LegIssuer" required="N" />
      <field name="EncodedLegIssuerLen" required="N" />
      <field name="EncodedLegIssuer" required="N" />
      <field name="LegSecurityDesc" required="N" />
      <field name="EncodedLegSecurityDescLen" required="N" />
      <field name="EncodedLegSecurityDesc" required="N" />
      <field name="LegRatioQty" required="N" />
      <field name="LegSide" required="N" />
    </component>
    <component name="OrderQtyData">
      <field name="OrderQty" required="Y" />
      <field name="CashOrderQty" required="N" />
      <field name="OrderPercent" required="N" />
      <field name="RoundingDirection" required="N" />
      <field name="RoundingModulus" required="N" />
    </component>
    <component name="CommissionData">
      <field name="Commission" required="N" />
      <field name="CommType" required="N" />
      <field name="CommCurrency" required="N" />
      <field name="FundRenewWaiv" required="N" />
    </component>
    <component name="Parties">
      <group name="NoPartyIDs" required="N">
        <field name="PartyID" required="N" />
        <field name="PartyIDSource" required="N" />
        <field name="PartyRole" required="N" />
        <field name="PartySubID" required="N" />
      </group>
    </component>
    <component name="NestedParties">
      <group name="NoNestedPartyIDs" required="N">
        <field name="NestedPartyID" required="N" />
        <field name="NestedPartyIDSource" required="N" />
        <field name="NestedPartyRole" required="N" />
        <field name="NestedPartySubID" required="N" />
      </group>
    </component>
    <component name="SpreadOrBenchmarkCurveData">
      <field name="Spread" required="N" />
      <field name="BenchmarkCurveCurrency" required="N" />
      <field name="BenchmarkCurveName" required="N" />
      <field name="BenchmarkCurvePoint" required="N" />
    </component>
    <component name="Stipulations">
      <group name="NoStipulations" required="N">
        <field name="StipulationType" required="N" />
        <field name="StipulationValue" required="N" />
      </group>
    </component>
    <component name="YieldData">
      <field name="YieldType" required="N" />
      <field name="Yield" required="N" />
    </component>
    <component name="UnderlyingInstrument">
      <field name="UnderlyingSymbol" required="Y" />
      <field name="UnderlyingSymbolSfx" required="N" />
      <field name="UnderlyingSecurityID" required="N" />
      <field name="UnderlyingSecurityIDSource" required="N" />
      <group name="NoUnderlyingSecurityAltID" required="N">
        <field name="UnderlyingSecurityAltID" required="N" />
        <field name="UnderlyingSecurityAltIDSource" required="N" />
      </group>
      <field name="UnderlyingProduct" required="N" />
      <field name="UnderlyingCFICode" required="N" />
      <field name="UnderlyingSecurityType" required="N" />
      <field name="UnderlyingMaturityMonthYear" required="N" />
      <field name="UnderlyingMaturityDate" required="N" />
      <field name="UnderlyingCouponPaymentDate" required="N" />
      <field name="UnderlyingIssueDate" required="N" />
      <field name="UnderlyingRepoCollateralSecurityType" required="N" />
      <field name="UnderlyingRepurchaseTerm" required="N" />
      <field name="UnderlyingRepurchaseRate" required="N" />
      <field name="UnderlyingFactor" required="N" />
      <field name="UnderlyingCreditRating" required="N" />
      <field name="UnderlyingInstrRegistry" required="N" />
      <field name="UnderlyingCountryOfIssue" required="N" />
      <field name="UnderlyingStateOrProvinceOfIssue" required="N" />
      <field name="UnderlyingLocaleOfIssue" required="N" />
      <field name="UnderlyingRedemptionDate" required="N" />
      <field name="UnderlyingPutOrCall" required="N" />
      <field name="UnderlyingStrikePrice" required="N" />
      <field name="UnderlyingOptAttribute" required="N" />
      <field name="UnderlyingContractMultiplier" required="N" />
      <field name="UnderlyingCouponRate" required="N" />
      <field name="UnderlyingSecurityExchange" required="N" />
      <field name="UnderlyingIssuer" required="N" />
      <field name="EncodedUnderlyingIssuerLen" required="N" />
      <field name="EncodedUnderlyingIssuer" required="N" />
      <field name="UnderlyingSecurityDesc" required="N" />
      <field name="EncodedUnderlyingSecurityDescLen" required="N" />
      <field name="EncodedUnderlyingSecurityDesc" required="N" />
    </component>
  </components>
  <fields>
    <field number="1" name="Account" type="STRING" />
    <field number="2" name="AdvId" type="STRING" />
    <field number="3" name="AdvRefID" type="STRING" />
    <field number="4" name="AdvSide" type="CHAR">
      <value enum="B" description="BUY" />
      <value enum="S" description="SELL" />
      <value enum="X" description="CROSS" />
      <value enum="T" description="TRADE" />
    </field>
    <field number="5" name="AdvTransType" type="STRING">
      <value enum="N" description="NEW" />
      <value enum="C" description="CANCEL" />
      <value enum="R" description="REPLACE" />
    </field>
    <field number="6" name="AvgPx" type="PRICE" />
    <field number="7" name="BeginSeqNo" type="SEQNUM" />
    <field number="8" name="BeginString" type="STRING" />
    <field number="9" name="BodyLength" type="LENGTH" />
    <field number="10" name="CheckSum" type="STRING" />
    <field number="11" name="ClOrdID" type="STRING" />
    <field number="12" name="Commission" type="AMT" />
    <field number="13" name="CommType" type="CHAR">
      <value enum="1" description="PER_SHARE" />
      <value enum="2" description="PERCENTAGE" />
      <value enum="3" description="ABSOLUTE" />
      <value enum="4" description="PERCENTAGE_WAIVED_CASH_DISCOUNT" />
      <value enum="5" description="PERCENTAGE_WAIVED_ENHANCED_UNITS" />
      <value enum="6" description="PER_BOND" />
    </field>
    <field number="14" name="CumQty" type="QTY" />
    <field number="15" name="Currency" type="CURRENCY" />
    <field number="16" name="EndSeqNo" type="SEQNUM" />
    <field number="17" name="ExecID" type="STRING" />
    <field number="18" name="ExecInst" type="MULTIPLEVALUESTRING">
      <value enum="1" description="NOT_HELD" />
      <value enum="2" description="WORK" />
      <value enum="3" description="GO_ALONG" />
      <value enum="4" description="OVER_THE_DAY" />
      <value enum="5" description="HELD" />
      <value enum="6" description="PARTICIPATE_DONT_INITIATE" />
      <value enum="7" description="STRICT_SCALE" />
      <value enum="8" description="TRY_TO_SCALE" />
      <value enum="9" description="STAY_ON_BIDSIDE" />
      <value enum="0" description="STAY_ON_OFFERSIDE" />
      <value enum="A" description="NO_CROSS" />
      <value enum="B" description="OK_TO_CROSS" />
      <value enum="C" description="CALL_FIRST" />
      <value enum="D" description="PERCENT_OF_VOLUME" />
      <value enum="E" description="DO_NOT_INCREASE" />
      <value enum="F" description="DO_NOT_REDUCE" />
      <value enum="G" description="ALL_OR_NONE" />
      <value enum="H" description="REINSTATE_ON_SYSTEM_FAILURE" />
      <value enum="I" description="INSTITUTIONS_ONLY" />
      <value enum="J" description="REINSTATE_ON_TRADING_HALT" />
      <value enum="K" description="CANCEL_ON_TRADING_HALT" />
      <value enum="L" description="LAST_PEG" />
      <value enum="M" description="MID_PRICE_PEG" />
      <value enum="N" description="NON_NEGOTIABLE" />
      <value enum="O" description="OPENING_PEG" />
      <value enum="P" description="MARKET_PEG" />
      <value enum="Q" description="CANCEL_ON_SYSTEM_FAILURE" />
      <value enum="R" description="PRIMARY_PEG" />
      <value enum="S" description="SUSPEND" />
      <value enum="T" description="FIXED_PEG" />
      <value enum="U" description="CUSTOMER_DISPLAY_INSTRUCTION" />
      <value enum="V" description="NETTING" />
      <value enum="W" description="PEG_TO_VWAP" />
      <value enum="X" description="TRADE_ALONG" />
      <value enum="Y" description="TRY_TO_STOP" />
      <value enum="ZT" description="Time priority" />
      <value enum="ZS" description="Size priority" />
      <value enum="ZP" description="Provider priority" />
    </field>
    <field number="19" name="ExecRefID" type="STRING" />
    <field number="20" name="ExecTransType" type="CHAR">
      <value enum="0" description="NEW" />
      <value enum="1" description="CANCEL" />
      <value enum="2" description="CORRECT" />
      <value enum="3" description="STATUS" />
    </field>
    <field number="21" name="HandlInst" type="CHAR">
      <value enum="1" description="AUTOMATED_EXECUTION_ORDER_PRIVATE" />
      <value enum="2" description="AUTOMATED_EXECUTION_ORDER_PUBLIC" />
      <value enum="3" description="MANUAL_ORDER" />
    </field>
    <field number="22" name="SecurityIDSource" type="STRING">
      <value enum="1" description="CUSIP" />
      <value enum="2" description="SEDOL" />
      <value enum="3" description="QUIK" />
      <value enum="4" description="ISIN_NUMBER" />
      <value enum="5" description="RIC_CODE" />
      <value enum="6" description="ISO_CURRENCY_CODE" />
      <value enum="7" description="ISO_COUNTRY_CODE" />
      <value enum="8" description="EXCHANGE_SYMBOL" />
      <value enum="9" description="CONSOLIDATED_TAPE_ASSOCIATION" />
      <value enum="A" description="BLOOMBERG_SYMBOL" />
      <value enum="B" description="WERTPAPIER" />
      <value enum="C" description="DUTCH" />
      <value enum="D" description="VALOREN" />
      <value enum="E" description="SICOVAM" />
      <value enum="F" description="BELGIAN" />
      <value enum="G" description="COMMON" />
    </field>
    <field number="23" name="IOIid" type="STRING" />
    <field number="25" name="IOIQltyInd" type="CHAR">
      <value enum="L" description="LOW" />
      <value enum="M" description="MEDIUM" />
      <value enum="H" description="HIGH" />
    </field>
    <field number="26" name="IOIRefID" type="STRING" />
    <field number="27" name="IOIQty" type="STRING" />
    <field number="28" name="IOITransType" type="CHAR">
      <value enum="N" description="NEW" />
      <value enum="C" description="CANCEL" />
      <value enum="R" description="REPLACE" />
    </field>
    <field number="29" name="LastCapacity" type="CHAR">
      <value enum="1" description="AGENT" />
      <value enum="2" description="CROSS_AS_AGENT" />
      <value enum="3" description="CROSS_AS_PRINCIPAL" />
      <value enum="4" description="PRINCIPAL" />
    </field>
    <field number="30" name="LastMkt" type="EXCHANGE" />
    <field number="31" name="LastPx" type="PRICE" />
    <field number="32" name="LastQty" type="QTY" />
    <field number="33" name="LinesOfText" type="NUMINGROUP" />
    <field number="34" name="MsgSeqNum" type="SEQNUM" />
    <field number="35" name="MsgType" type="STRING">
      <value enum="0" description="HEARTBEAT" />
      <value enum="1" description="TEST_REQUEST" />
      <value enum="2" description="RESEND_REQUEST" />
      <value enum="3" description="REJECT" />
      <value enum="4" description="SEQUENCE_RESET" />
      <value enum="5" description="LOGOUT" />
      <value enum="6" description="INDICATION_OF_INTEREST" />
      <value enum="7" description="ADVERTISEMENT" />
      <value enum="8" description="EXECUTION_REPORT" />
      <value enum="9" description="ORDER_CANCEL_REJECT" />
      <value enum="A" description="LOGON" />
      <value enum="B" description="NEWS" />
      <value enum="C" description="EMAIL" />
      <value enum="D" description="ORDER_SINGLE" />
      <value enum="E" description="ORDER_LIST" />
      <value enum="F" description="ORDER_CANCEL_REQUEST" />
      <value enum="G" description="ORDER_CANCEL" />
      <value enum="H" description="ORDER_STATUS_REQUEST" />
      <value enum="J" description="ALLOCATION" />
      <value enum="K" description="LIST_CANCEL_REQUEST" />
      <value enum="L" description="LIST_EXECUTE" />
      <value enum="M" description="LIST_STATUS_REQUEST" />
      <value enum="N" description="LIST_STATUS" />
      <value enum="P" description="ALLOCATION_ACK" />
      <value enum="Q" description="DONT_KNOW_TRADE" />
      <value enum="R" description="QUOTE_REQUEST" />
      <value enum="S" description="QUOTE" />
      <value enum="T" description="SETTLEMENT_INSTRUCTIONS" />
      <value enum="V" description="MARKET_DATA_REQUEST" />
      <value enum="W" description="MARKET_DATA_SNAPSHOT" />
      <value enum="X" description="MARKET_DATA_INCREMENTAL_REFRESH" />
      <value enum="Y" description="MARKET_DATA_REQUEST_REJECT" />
      <value enum="Z" description="QUOTE_CANCEL" />
      <value enum="a" description="QUOTE_STATUS_REQUEST" />
      <value enum="b" description="MASS_QUOTE_ACKNOWLEDGEMENT" />
      <value enum="c" description="SECURITY_DEFINITION_REQUEST" />
      <value enum="d" description="SECURITY_DEFINITION" />
      <value enum="e" description="SECURITY_STATUS_REQUEST" />
      <value enum="f" description="SECURITY_STATUS" />
      <value enum="g" description="TRADING_SESSION_STATUS_REQUEST" />
      <value enum="h" description="TRADING_SESSION_STATUS" />
      <value enum="i" description="MASS_QUOTE" />
      <value enum="j" description="BUSINESS_MESSAGE_REJECT" />
      <value enum="k" description="BID_REQUEST" />
      <value enum="l" description="BID_RESPONSE" />
      <value enum="m" description="LIST_STRIKE_PRICE" />
      <value enum="n" description="XML_MESSAGE" />
      <value enum="o" description="REGISTRATION_INSTRUCTIONS" />
      <value enum="p" description="REGISTRATION_INSTRUCTIONS_RESPONSE" />
      <value enum="q" description="ORDER_MASS_CANCEL_REQUEST" />
      <value enum="r" description="ORDER_MASS_CANCEL_REPORT" />
      <value enum="s" description="NEW_ORDER_CROSS" />
      <value enum="t" description="CROSS_ORDER_CANCEL" />
      <value enum="u" description="CROSS_ORDER_CANCEL_REQUEST" />
      <value enum="v" description="SECURITY_TYPE_REQUEST" />
      <value enum="w" description="SECURITY_TYPES" />
      <value enum="x" description="SECURITY_LIST_REQUEST" />
      <value enum="y" description="SECURITY_LIST" />
      <value enum="z" description="DERIVATIVE_SECURITY_LIST_REQUEST" />
      <value enum="AA" description="DERIVATIVE_SECURITY_LIST" />
      <value enum="AB" description="NEW_ORDER_MULTILEG" />
      <value enum="AC" description="MULTILEG_ORDER_CANCEL" />
      <value enum="AD" description="TRADE_CAPTURE_REPORT_REQUEST" />
      <value enum="AE" description="TRADE_CAPTURE_REPORT" />
      <value enum="AF" description="ORDER_MASS_STATUS_REQUEST" />
      <value enum="AG" description="QUOTE_REQUEST_REJECT" />
      <value enum="AH" description="RFQ_REQUEST" />
      <value enum="AI" description="QUOTE_STATUS_REPORT" />
    </field>
    <field number="36" name="NewSeqNo" type="SEQNUM" />
    <field number="37" name="OrderID" type="STRING" />
    <field number="38" name="OrderQty" type="QTY" />
    <field number="39" name="OrdStatus" type="CHAR">
      <value enum="0" description="NEW" />
      <value enum="1" description="PARTIALLY_FILLED" />
      <value enum="2" description="FILLED" />
      <value enum="3" description="DONE_FOR_DAY" />
      <value enum="4" description="CANCELED" />
      <value enum="5" description="REPLACED" />
      <value enum="6" description="PENDING_CANCEL" />
      <value enum="7" description="STOPPED" />
      <value enum="8" description="REJECTED" />
      <value enum="9" description="SUSPENDED" />
      <value enum="A" description="PENDING_NEW" />
      <value enum="B" description="CALCULATED" />
      <value enum="C" description="EXPIRED" />
      <value enum="D" description="ACCEPTED_FOR_BIDDING" />
      <value enum="E" description="PENDING_REPLACE" />
    </field>
    <field number="40" name="OrdType" type="CHAR">
      <value enum="1" description="MARKET" />
      <value enum="2" description="LIMIT" />
      <value enum="3" description="STOP" />
      <value enum="4" description="STOP_LIMIT" />
      <value enum="5" description="MARKET_ON_CLOSE" />
      <value enum="6" description="WITH_OR_WITHOUT" />
      <value enum="7" description="LIMIT_OR_BETTER" />
      <value enum="8" description="LIMIT_WITH_OR_WITHOUT" />
      <value enum="9" description="ON_BASIS" />
      <value enum="A" description="ON_CLOSE" />
      <value enum="B" description="LIMIT_ON_CLOSE" />
      <value enum="C" description="FOREX_MARKET" />
      <value enum="D" description="PREVIOUSLY_QUOTED" />
      <value enum="E" description="PREVIOUSLY_INDICATED" />
      <value enum="F" description="FOREX_LIMIT" />
      <value enum="G" description="FOREX_SWAP" />
      <value enum="H" description="FOREX_PREVIOUSLY_QUOTED" />
      <value enum="I" description="FUNARI" />
      <value enum="J" description="MARKET_IF_TOUCHED" />
      <value enum="K" description="MARKET_WITH_LEFTOVER_AS_LIMIT" />
      <value enum="L" description="PREVIOUS_FUND_VALUATION_POINT" />
      <value enum="M" description="NEXT_FUND_VALUATION_POINT" />
      <value enum="P" description="PEGGED" />
    </field>
    <field number="41" name="OrigClOrdID" type="STRING" />
    <field number="42" name="OrigTime" type="UTCTIMESTAMP" />
    <field number="43" name="PossDupFlag" type="BOOLEAN" />
    <field number="44" name="Price" type="PRICE" />
    <field number="45" name="RefSeqNum" type="SEQNUM" />
    <field number="46" name="RelatdSym" type="STRING" />
    <field type="CHAR" name="Rule80A" number="47">
      <value enum="A" description="AGENCY_SINGLE_ORDER" />
      <value enum="B" description="SHORT_EXEMPT_TRANSACTION_REFER_TO_A_TYPE" />
      <value enum="C" description="PROGRAM_ORDER_NON_INDEX_ARB_FOR_MEMBER_FIRM" />
      <value enum="D" description="PROGRAM_ORDER_INDEX_ARB_FOR_MEMBER_FIRM" />
      <value enum="E" description="SHORT_EXEMPT_TRANSACTION_FOR_PRINCIPAL" />
      <value enum="F" description="SHORT_EXEMPT_TRANSACTION_REFER_TO_W_TYPE" />
      <value enum="H" description="SHORT_EXEMPT_TRANSACTION_REFER_TO_I_TYPE" />
      <value enum="I" description="INDIVIDUAL_INVESTOR" />
      <value enum="J" description="PROGRAM_ORDER_INDEX_ARB_FOR_INDIVIDUAL_CUSTOMER" />
      <value enum="K" description="PROGRAM_ORDER_NON_INDEX_ARB_FOR_INDIVIDUAL_CUSTOMER" />
      <value enum="L" description="SHORT_EXEMPT_AFFILIATED" />
      <value enum="M" description="PROGRAM_ORDER_INDEX_ARB_FOR_OTHER_MEMBER" />
      <value enum="N" description="PROGRAM_ORDER_NON_INDEX_ARB_FOR_OTHER_MEMBER" />
      <value enum="O" description="PROPRIETARY_AFFILIATED" />
      <value enum="P" description="PRINCIPAL" />
      <value enum="R" description="TRANSACTIONS_NON_MEMBER" />
      <value enum="S" description="SPECIALIST_TRADES" />
      <value enum="T" description="TRANSACTIONS_UNAFFILIATED_MEMBER" />
      <value enum="U" description="PROGRAM_ORDER_INDEX_ARB_FOR_OTHER_AGENCY" />
      <value enum="W" description="ALL_OTHER_ORDERS_AS_AGENT_FOR_OTHER_MEMBER" />
      <value enum="X" description="SHORT_EXEMPT_NOT_AFFILIATED" />
      <value enum="Y" description="PROGRAM_ORDER_NON_INDEX_ARB_FOR_OTHER_AGENCY" />
      <value enum="Z" description="SHORT_EXEMPT_NONMEMBER" />
    </field>
    <field number="48" name="SecurityID" type="STRING" />
    <field number="49" name="SenderCompID" type="STRING" />
    <field number="50" name="SenderSubID" type="STRING" />
    <field number="51" name="SendingDate" type="LOCALMKTDATE" />
    <field number="52" name="SendingTime" type="UTCTIMESTAMP" />
    <field number="53" name="Quantity" type="QTY" />
    <field number="54" name="Side" type="CHAR">
      <value enum="1" description="BUY" />
      <value enum="2" description="SELL" />
	  <value enum=" " description="TWO_WAY" />
      <value enum="3" description="BUY_MINUS" />
      <value enum="4" description="SELL_PLUS" />
      <value enum="5" description="SELL_SHORT" />
      <value enum="6" description="SELL_SHORT_EXEMPT" />
      <value enum="7" description="UNDISCLOSED" />
      <value enum="8" description="CROSS" />
      <value enum="9" description="CROSS_SHORT" />
      <value enum="A" description="CROSS_SHORT_EXEMPT" />
      <value enum="B" description="AS_DEFINED" />
      <value enum="C" description="OPPOSITE" />
    </field>
    <field number="55" name="Symbol" type="STRING" />
    <field number="56" name="TargetCompID" type="STRING" />
    <field number="57" name="TargetSubID" type="STRING" />
    <field number="58" name="Text" type="STRING" />
    <field number="59" name="TimeInForce" type="CHAR">
      <value enum="0" description="DAY" />
      <value enum="1" description="GOOD_TILL_CANCEL" />
      <value enum="2" description="AT_THE_OPENING" />
      <value enum="3" description="IMMEDIATE_OR_CANCEL" />
      <value enum="4" description="FILL_OR_KILL" />
      <value enum="5" description="GOOD_TILL_CROSSING" />
      <value enum="6" description="GOOD_TILL_DATE" />
      <value enum="7" description="AT_THE_CLOSE" />
    </field>
    <field number="60" name="TransactTime" type="UTCTIMESTAMP" />
    <field number="61" name="Urgency" type="CHAR">
      <value enum="0" description="NORMAL" />
      <value enum="1" description="FLASH" />
      <value enum="2" description="BACKGROUND" />
    </field>
    <field number="62" name="ValidUntilTime" type="UTCTIMESTAMP" />
    <field number="63" name="SettlmntTyp" type="CHAR">
      <value enum="0" description="REGULAR" />
      <value enum="1" description="CASH" />
      <value enum="2" description="NEXT_DAY" />
      <value description="T_PLUS_2" enum="3" />
      <value description="T_PLUS_3" enum="4" />
      <value description="T_PLUS_4" enum="5" />
      <value enum="6" description="FUTURE" />
      <value enum="7" description="WHEN_AND_IF_ISSUED" />
      <value enum="8" description="SELLERS_OPTION" />
      <value description="T_PLUS_5" enum="9" />
      <value description="T_PLUS_1" enum="A" />
    </field>
    <field number="64" name="FutSettDate" type="LOCALMKTDATE" />
    <field number="65" name="SymbolSfx" type="STRING" />
    <field number="66" name="ListID" type="STRING" />
    <field number="67" name="ListSeqNo" type="INT" />
    <field number="68" name="TotNoOrders" type="INT" />
    <field number="69" name="ListExecInst" type="STRING" />
    <field number="70" name="AllocID" type="STRING" />
    <field number="71" name="AllocTransType" type="CHAR">
      <value enum="0" description="NEW" />
      <value enum="1" description="REPLACE" />
      <value enum="2" description="CANCEL" />
      <value enum="3" description="PRELIMINARY" />
      <value enum="4" description="CALCULATED" />
      <value enum="5" description="CALCULATED_WITHOUT_PRELIMINARY" />
    </field>
    <field number="72" name="RefAllocID" type="STRING" />
    <field number="73" name="NoOrders" type="NUMINGROUP" />
    <field number="74" name="AvgPrxPrecision" type="INT" />
    <field number="75" name="TradeDate" type="LOCALMKTDATE" />
    <field number="76" name="ExecBroker" type="STRING" />
    <field number="77" name="PositionEffect" type="CHAR" />
    <field number="78" name="NoAllocs" type="NUMINGROUP" />
    <field number="79" name="AllocAccount" type="STRING" />
    <field number="80" name="AllocQty" type="QTY" />
    <field number="81" name="ProcessCode" type="CHAR">
      <value enum="0" description="REGULAR" />
      <value enum="1" description="SOFT_DOLLAR" />
      <value enum="2" description="STEP_IN" />
      <value enum="3" description="STEP_OUT" />
      <value enum="4" description="SOFT_DOLLAR_STEP_IN" />
      <value enum="5" description="SOFT_DOLLAR_STEP_OUT" />
      <value enum="6" description="PLAN_SPONSOR" />
    </field>
    <field number="82" name="NoRpts" type="NUMINGROUP" />
    <field number="83" name="RptSeq" type="INT" />
    <field number="84" name="CxlQty" type="QTY" />
    <field number="85" name="NoDlvyInst" type="INT" />
    <field number="86" name="DlvyInst" type="STRING" />
    <field number="87" name="AllocStatus" type="INT">
      <value enum="0" description="ACCEPTED" />
      <value enum="1" description="REJECTED" />
      <value enum="2" description="PARTIAL_ACCEPT" />
      <value enum="3" description="RECEIVED" />
    </field>
    <field number="88" name="AllocRejCode" type="INT">
      <value enum="0" description="UNKNOWN_ACCOUNT" />
      <value enum="1" description="INCORRECT_QUANTITY" />
      <value enum="2" description="INCORRECT_AVERAGE_PRICE" />
      <value enum="3" description="UNKNOWN_EXECUTING_BROKER_MNEMONIC" />
      <value enum="4" description="COMMISSION_DIFFERENCE" />
      <value enum="5" description="UNKNOWN_ORDERID" />
      <value enum="6" description="UNKNOWN_LISTID" />
      <value enum="7" description="OTHER" />
    </field>
    <field number="89" name="Signature" type="DATA" />
    <field number="90" name="SecureDataLen" type="LENGTH" />
    <field number="91" name="SecureData" type="DATA" />
    <field number="92" name="BrokerOfCredit" type="STRING" />
    <field number="93" name="SignatureLength" type="LENGTH" />
    <field number="94" name="EmailType" type="CHAR">
      <value enum="0" description="NEW" />
      <value enum="1" description="REPLY" />
      <value enum="2" description="ADMIN_REPLY" />
    </field>
    <field number="95" name="RawDataLength" type="LENGTH" />
    <field number="96" name="RawData" type="DATA" />
    <field number="97" name="PossResend" type="BOOLEAN" />
    <field number="98" name="EncryptMethod" type="INT">
      <value enum="0" description="NONE" />
      <value enum="1" description="PKCS_PROPRIETARY" />
      <value enum="2" description="DES" />
      <value enum="3" description="PKCS_DES" />
      <value enum="4" description="PGP_DES" />
      <value enum="5" description="PGP_DES_MD5" />
      <value enum="6" description="PEM" />
    </field>
    <field number="99" name="StopPx" type="PRICE" />
    <field number="100" name="ExDestination" type="EXCHANGE" />
    <field number="102" name="CxlRejReason" type="INT">
      <value enum="0" description="TOO_LATE_TO_CANCEL" />
      <value enum="1" description="UNKNOWN_ORDER" />
      <value enum="2" description="BROKER" />
      <value enum="3" description="ALREADY_PENDING" />
      <value enum="4" description="UNABLE_TO_PROCESS_ORDER_MASS_CANCEL_REQUEST" />
      <value enum="5" description="ORIGORDMODTIME_DID_NOT_MATCH_LAST_TRANSACTTIME_OF_ORDER" />
      <value enum="6" description="DUPLICATE_CLORDID_RECEIVED" />
	  <value enum="99" description="OTHER" />
    </field>
    <field number="103" name="OrdRejReason" type="INT">
      <value enum="0" description="BROKER_OPTION" />
      <value enum="1" description="UNKNOWN_SYMBOL" />
      <value enum="2" description="EXCHANGE_CLOSED" />
      <value enum="3" description="ORDER_EXCEEDS_LIMIT" />
      <value enum="4" description="TOO_LATE_TO_ENTER" />
      <value enum="5" description="UNKNOWN_ORDER" />
      <value enum="6" description="DUPLICATE_ORDER" />
      <value enum="7" description="DUPLICATE_VERBAL" />
      <value enum="8" description="STALE_ORDER" />
      <value enum="9" description="TRADE_ALONG_REQUIRED" />
      <value enum="10" description="INVALID_INVESTOR_ID" />
      <value enum="11" description="UNSUPPORTED_ORDER_CHARACTERISTIC" />
      <value enum="12" description="SURVEILLENCE_OPTION" />
    </field>
    <field number="104" name="IOIQualifier" type="CHAR">
      <value enum="A" description="ALL_OR_NONE" />
      <value enum="B" description="MARKET_ON_CLOSE" />
      <value enum="C" description="AT_THE_CLOSE" />
      <value enum="D" description="VWAP" />
      <value enum="I" description="IN_TOUCH_WITH" />
      <value enum="L" description="LIMIT" />
      <value enum="M" description="MORE_BEHIND" />
      <value enum="O" description="AT_THE_OPEN" />
      <value enum="P" description="TAKING_A_POSITION" />
      <value enum="Q" description="AT_THE_MARKET" />
      <value enum="R" description="READY_TO_TRADE" />
      <value enum="S" description="PORTFOLIO_SHOWN" />
      <value enum="T" description="THROUGH_THE_DAY" />
      <value enum="V" description="VERSUS" />
      <value enum="W" description="INDICATION" />
      <value enum="X" description="CROSSING_OPPORTUNITY" />
      <value enum="Y" description="AT_THE_MIDPOINT" />
      <value enum="Z" description="PRE_OPEN" />
    </field>
    <field number="105" name="WaveNo" type="STRING" />
    <field number="106" name="Issuer" type="STRING" />
    <field number="107" name="SecurityDesc" type="STRING" />
    <field number="108" name="HeartBtInt" type="INT" />
    <field number="109" name="ClientID" type="STRING" />
    <field number="110" name="MinQty" type="QTY" />
    <field number="111" name="MaxFloor" type="QTY" />
    <field number="112" name="TestReqID" type="STRING" />
    <field number="113" name="ReportToExch" type="BOOLEAN" />
    <field number="114" name="LocateReqd" type="BOOLEAN" />
    <field number="115" name="OnBehalfOfCompID" type="STRING" />
    <field number="116" name="OnBehalfOfSubID" type="STRING" />
    <field number="117" name="QuoteID" type="STRING" />
    <field number="118" name="NetMoney" type="AMT" />
    <field number="119" name="SettlCurrAmt" type="AMT" />
    <field number="120" name="SettlCurrency" type="CURRENCY" />
    <field number="121" name="ForexReq" type="BOOLEAN" />
    <field number="122" name="OrigSendingTime" type="UTCTIMESTAMP" />
    <field number="123" name="GapFillFlag" type="BOOLEAN" />
    <field number="124" name="NoExecs" type="NUMINGROUP" />
    <field number="125" name="CxlType" type="CHAR" />
    <field number="126" name="ExpireTime" type="UTCTIMESTAMP" />
    <field number="127" name="DKReason" type="CHAR">
      <value enum="A" description="UNKNOWN_SYMBOL" />
      <value enum="B" description="WRONG_SIDE" />
      <value enum="C" description="QUANTITY_EXCEEDS_ORDER" />
      <value enum="D" description="NO_MATCHING_ORDER" />
      <value enum="E" description="PRICE_EXCEEDS_LIMIT" />
      <value enum="Z" description="OTHER" />
    </field>
    <field number="128" name="DeliverToCompID" type="STRING" />
    <field number="129" name="DeliverToSubID" type="STRING" />
    <field number="130" name="IOINaturalFlag" type="BOOLEAN" />
    <field number="131" name="QuoteReqID" type="STRING" />
    <field number="132" name="BidPx" type="PRICE" />
    <field number="133" name="OfferPx" type="PRICE" />
    <field number="134" name="BidSize" type="QTY" />
    <field number="135" name="OfferSize" type="QTY" />
    <field number="136" name="NoMiscFees" type="NUMINGROUP" />
    <field number="137" name="MiscFeeAmt" type="AMT" />
    <field number="138" name="MiscFeeCurr" type="CURRENCY" />
    <field number="139" name="MiscFeeType" type="CHAR">
      <value enum="1" description="REGULATORY" />
      <value enum="2" description="TAX" />
      <value enum="3" description="LOCAL_COMMISSION" />
      <value enum="4" description="EXCHANGE_FEES" />
      <value enum="5" description="STAMP" />
      <value enum="6" description="LEVY" />
      <value enum="7" description="OTHER" />
      <value enum="8" description="MARKUP" />
      <value enum="9" description="CONSUMPTION_TAX" />
    </field>
    <field number="140" name="PrevClosePx" type="PRICE" />
    <field number="141" name="ResetSeqNumFlag" type="BOOLEAN" />
    <field number="142" name="SenderLocationID" type="STRING" />
    <field number="143" name="TargetLocationID" type="STRING" />
    <field number="144" name="OnBehalfOfLocationID" type="STRING" />
    <field number="145" name="DeliverToLocationID" type="STRING" />
    <field number="146" name="NoRelatedSym" type="NUMINGROUP" />
    <field number="147" name="Subject" type="STRING" />
    <field number="148" name="Headline" type="STRING" />
    <field number="149" name="URLLink" type="STRING" />
    <field number="150" name="ExecType" type="CHAR">
      <value enum="0" description="NEW" />
      <value enum="1" description="PARTIAL_FILL" />
      <value enum="2" description="FILL" />
      <value enum="3" description="DONE_FOR_DAY" />
      <value enum="4" description="CANCELED" />
      <value enum="5" description="REPLACE" />
      <value enum="6" description="PENDING_CANCEL" />
      <value enum="7" description="STOPPED" />
      <value enum="8" description="REJECTED" />
      <value enum="9" description="SUSPENDED" />
      <value enum="A" description="PENDING_NEW" />
      <value enum="B" description="CALCULATED" />
      <value enum="C" description="EXPIRED" />
      <value enum="D" description="RESTATED" />
      <value enum="E" description="PENDING_REPLACE" />
      <value enum="F" description="TRADE" />
      <value enum="G" description="TRADE_CORRECT" />
      <value enum="H" description="TRADE_CANCEL" />
      <value enum="I" description="ORDER_STATUS" />
    </field>
    <field number="151" name="LeavesQty" type="QTY" />
    <field number="152" name="CashOrderQty" type="QTY" />
    <field number="153" name="AllocAvgPx" type="PRICE" />
    <field number="154" name="AllocNetMoney" type="AMT" />
    <field number="155" name="SettlCurrFxRate" type="FLOAT" />
    <field number="156" name="SettlCurrFxRateCalc" type="CHAR" />
    <field number="157" name="NumDaysInterest" type="INT" />
    <field number="158" name="AccruedInterestRate" type="PERCENTAGE" />
    <field number="159" name="AccruedInterestAmt" type="AMT" />
    <field number="160" name="SettlInstMode" type="CHAR">
      <value enum="0" description="DEFAULT" />
      <value enum="1" description="STANDING_INSTRUCTIONS_PROVIDED" />
      <value enum="2" description="SPECIFIC_ALLOCATION_ACCOUNT_OVERRIDING" />
      <value enum="3" description="SPECIFIC_ALLOCATION_ACCOUNT_STANDING" />
      <value enum="4" description="SPECIFIC_ORDER_FOR_A_SINGLE_ACCOUNT" />
    </field>
    <field number="161" name="AllocText" type="STRING" />
    <field number="162" name="SettlInstID" type="STRING" />
    <field number="163" name="SettlInstTransType" type="CHAR">
      <value enum="N" description="NEW" />
      <value enum="C" description="CANCEL" />
      <value enum="R" description="REPLACE" />
    </field>
    <field number="164" name="EmailThreadID" type="STRING" />
    <field number="165" name="SettlInstSource" type="CHAR">
      <value enum="1" description="BROKERS_INSTRUCTIONS" />
      <value enum="2" description="INSTITUTIONS_INSTRUCTIONS" />
      <value enum="3" description="INVESTOR" />
    </field>
    <field number="166" name="SettlLocation" type="STRING">
      <value enum="CED" description="CEDEL" />
      <value enum="DTC" description="DEPOSITORY_TRUST_COMPANY" />
      <value enum="EUR" description="EUROCLEAR" />
      <value enum="FED" description="FEDERAL_BOOK_ENTRY" />
      <value enum="PED" description="PHYSICAL" />
      <value enum="PTC" description="PARTICIPANT_TRUST_COMPANY_ISO_COUNTRY" />
    </field>
    <field number="167" name="SecurityType" type="STRING">
      <value enum="CORP" description="CORPORATE_BOND" />
      <value enum="CPP" description="CORPORATE_PRIVATE_PLACEMENT" />
      <value enum="CB" description="CONVERTABLE_BOND" />
      <value enum="DUAL" description="DUAL_CURRENCY" />
      <value enum="XLINKD" description="INDEX_LINKED" />
      <value enum="STRUCT" description="STRUCTURED_NOTES" />
      <value enum="YANK" description="YANKEE_CORPORATE_BOND" />
      <value enum="FOR" description="FOREIGN_EXCHANGE_CONTRACT" />
      <value enum="CS" description="COMMON_STOCK" />
      <value enum="PS" description="PREFERED_STOCK" />
      <value enum="BRADY" description="BRADY_BOND" />
      <value enum="TBOND" description="US_TREASURY_BOND" />
      <value enum="TINT" description="INTEREST_STRIP_FROM_ANY_BOND_OR_NOTE" />
      <value enum="TIPS" description="TREASURY_INFLATION_PROTECTED_SECURITIES" />
      <value enum="TCAL" description="PRINCIPAL_STRIP_OF_A_CALLABLE_BOND_OR_NOTE" />
      <value enum="TPRN" description="PRINCIPAL_STRIP_FROM_A_NON_CALLABLE_BOND_OR_NOTE" />
      <value enum="UST" description="US_TREASURY_NOTE_BOND" />
      <value enum="USTB" description="US_TREASURY_BILL" />
      <value enum="TERM" description="TERM_LOAN" />
      <value enum="RVLV" description="REVOLVER_LOAN" />
      <value enum="RVLVTRM" description="REVOLVER_TERM_LOAN" />
      <value enum="BRIDGE" description="BRIDGE_LOAN" />
      <value enum="LOFC" description="LETTER_OF_CREDIT" />
      <value enum="SWING" description="SWING_LINE_FACILITY" />
      <value enum="DINP" description="DEBTOR_IN_POSSESSION" />
      <value enum="DEFLTED" description="DEFAULTED" />
      <value enum="WITHDRN" description="WITHDRAWN" />
      <value enum="REPLACD" description="REPLACED" />
      <value enum="MATURED" description="MATURED" />
      <value enum="AMENDED" description="AMENDED_AND_RESTATED" />
      <value enum="RETIRED" description="RETIRED" />
      <value enum="BA" description="BANKERS_ACCEPTANCE" />
      <value enum="BN" description="BANK_NOTES" />
      <value enum="BOX" description="BILL_OF_EXCHANGES" />
      <value enum="CD" description="CERTIFICATE_OF_DEPOSIT" />
      <value enum="CL" description="CALL_LOANS" />
      <value enum="CP" description="COMMERCIAL_PAPER" />
      <value enum="DP" description="DEPOSIT_NOTES" />
      <value enum="LQN" description="LIQUIDITY_NOTES" />
      <value enum="MTN" description="MEDIUM_TERM_NOTES" />
      <value enum="ONITE" description="OVERNITE" />
      <value enum="PN" description="PROMISSORY_NOTES" />
      <value enum="PZFJ" description="PLAZOS_FIJOS" />
      <value enum="RP" description="REPURCHASE_AGREEMENT" />
      <value enum="RVRP" description="REVERSE_REPURCHASE_AGREEMENT" />
      <value enum="STN" description="SHORT_TERM_LOAN_NOTE" />
      <value enum="TD" description="TIME_DEPOSIT" />
      <value enum="XCN" description="EXTENDED_COMM_NOTE" />
      <value enum="POOL" description="AGENCY_POOLS" />
      <value enum="ABS" description="ASSET_BACKED_SECURITIES" />
      <value enum="CMBS" description="CORP_MORTGAGE_BACKED_SECURITIES" />
      <value enum="CMO" description="COLLATERALIZED_MORTGAGE_OBLIGATION" />
      <value enum="IET" description="IOETTE_MORTGAGE" />
      <value enum="MBS" description="MORTGAGE_BACKED_SECURITIES" />
      <value enum="MIO" description="MORTGAGE_INTEREST_ONLY" />
      <value enum="MPO" description="MORTGAGE_PRINCIPAL_ONLY" />
      <value enum="MPP" description="MORTGAGE_PRIVATE_PLACEMENT" />
      <value enum="MPT" description="MISCELLANEOUS_PASS_THROUGH" />
      <value enum="AN" description="OTHER_ANTICIPATION_NOTES" />
      <value enum="COFO" description="CERTIFICATE_OF_OBLIGATION" />
      <value enum="COFP" description="CERTIFICATE_OF_PARTICIPATION" />
      <value enum="GO" description="GENERAL_OBLIGATION_BONDS" />
      <value enum="MT" description="MANDATORY_TENDER" />
      <value enum="RAN" description="REVENUE_ANTICIPATION_NOTE" />      
      <value enum="REV" description="REVENUE_BONDS" />
      <value enum="SPCLA" description="SPECIAL_ASSESSMENT" />
      <value enum="SPCLO" description="SPECIAL_OBLIGATION" />
      <value enum="SPCLT" description="SPECIAL_TAX" />
      <value enum="TAN" description="TAX_ANTICIPATION_NOTE" />
      <value enum="TAXA" description="TAX_ALLOCATION" />
      <value enum="TECP" description="TAX_EXEMPT_COMMERCIAL_PAPER" />
      <value enum="TRAN" description="TAX_AND_REVENUE_ANTICIPATION_NOTE" />
      <value enum="VRDN" description="VARIABLE_RATE_DEMAND_NOTE" />
      <value enum="WAR" description="WARRANT" />
      <value enum="MF" description="MUTUAL_FUND" />
      <value enum="MLEG" description="MULTI_LEG_INSTRUMENT" />
      <value enum="NONE" description="NO_SECURITY_TYPE" />
      <value description="WILDCARD" enum="?" />
    </field>
    <field number="168" name="EffectiveTime" type="UTCTIMESTAMP" />
    <field number="169" name="StandInstDbType" type="INT">
      <value enum="0" description="OTHER" />
      <value enum="1" description="DTC_SID" />
      <value enum="2" description="THOMSON_ALERT" />
      <value enum="3" description="A_GLOBAL_CUSTODIAN" />
    </field>
    <field number="170" name="StandInstDbName" type="STRING" />
    <field number="171" name="StandInstDbID" type="STRING" />
    <field number="172" name="SettlDeliveryType" type="INT" />
    <field number="173" name="SettlDepositoryCode" type="STRING" />
    <field number="174" name="SettlBrkrCode" type="STRING" />
    <field number="175" name="SettlInstCode" type="STRING" />
    <field number="176" name="SecuritySettlAgentName" type="STRING" />
    <field number="177" name="SecuritySettlAgentCode" type="STRING" />
    <field number="178" name="SecuritySettlAgentAcctNum" type="STRING" />
    <field number="179" name="SecuritySettlAgentAcctName" type="STRING" />
    <field number="180" name="SecuritySettlAgentContactName" type="STRING" />
    <field number="181" name="SecuritySettlAgentContactPhone" type="STRING" />
    <field number="182" name="CashSettlAgentName" type="STRING" />
    <field number="183" name="CashSettlAgentCode" type="STRING" />
    <field number="184" name="CashSettlAgentAcctNum" type="STRING" />
    <field number="185" name="CashSettlAgentAcctName" type="STRING" />
    <field number="186" name="CashSettlAgentContactName" type="STRING" />
    <field number="187" name="CashSettlAgentContactPhone" type="STRING" />
    <field number="188" name="BidSpotRate" type="PRICE" />
    <field number="189" name="BidForwardPoints" type="PRICEOFFSET" />
    <field number="190" name="OfferSpotRate" type="PRICE" />
    <field number="191" name="OfferForwardPoints" type="PRICEOFFSET" />
    <field type="QTY" name="OrderQty2" number="192" />
    <field type="LOCALMKTDATE" name="FutSettDate2" number="193" />
    <field number="194" name="LastSpotRate" type="PRICE" />
    <field number="195" name="LastForwardPoints" type="PRICEOFFSET" />
    <field number="196" name="AllocLinkID" type="STRING" />
    <field number="197" name="AllocLinkType" type="INT">
      <value enum="0" description="F_X_NETTING" />
      <value enum="1" description="F_X_SWAP" />
    </field>
    <field number="198" name="SecondaryOrderID" type="STRING" />
    <field number="199" name="NoIOIQualifiers" type="NUMINGROUP" />
    <field number="200" name="MaturityMonthYear" type="MONTHYEAR" />
    <field number="201" name="PutOrCall" type="INT">
      <value enum="0" description="PUT" />
      <value enum="1" description="CALL" />
    </field>
    <field number="202" name="StrikePrice" type="PRICE" />
    <field number="203" name="CoveredOrUncovered" type="INT">
      <value enum="0" description="COVERED" />
      <value enum="1" description="UNCOVERED" />
    </field>
    <field number="204" name="CustomerOrFirm" type="INT">
      <value enum="0" description="CUSTOMER" />
      <value enum="1" description="FIRM" />
    </field>
    <field number="205" name="MaturityDay" type="DAYOFMONTH" />
    <field number="206" name="OptAttribute" type="CHAR" />
    <field number="207" name="SecurityExchange" type="EXCHANGE" />
    <field number="208" name="NotifyBrokerOfCredit" type="BOOLEAN" />
    <field number="209" name="AllocHandlInst" type="INT">
      <value enum="1" description="MATCH" />
      <value enum="2" description="FORWARD" />
      <value enum="3" description="FORWARD_AND_MATCH" />
    </field>
    <field number="210" name="MaxShow" type="QTY" />
    <field number="211" name="PegDifference" type="PRICEOFFSET" />
    <field number="212" name="XmlDataLen" type="LENGTH" />
    <field number="213" name="XmlData" type="DATA" />
    <field number="214" name="SettlInstRefID" type="STRING" />
    <field number="215" name="NoRoutingIDs" type="NUMINGROUP" />
    <field number="216" name="RoutingType" type="INT">
      <value enum="1" description="TARGET_FIRM" />
      <value enum="2" description="TARGET_LIST" />
      <value enum="3" description="BLOCK_FIRM" />
      <value enum="4" description="BLOCK_LIST" />
    </field>
    <field number="217" name="RoutingID" type="STRING" />
    <field number="218" name="Spread" type="PRICEOFFSET" />
    <field number="219" name="Benchmark" type="CHAR">
      <value enum="1" description="CURVE" />
      <value enum="2" description="FIVEYR" />
      <value enum="3" description="OLD5" />
      <value enum="4" description="TENYR" />
      <value enum="5" description="OLD10" />
      <value enum="6" description="THIRTYYR" />
      <value enum="7" description="OLD30" />
      <value enum="8" description="THREEMOLIBOR" />
      <value enum="9" description="SIXMOLIBOR" />
    </field>
    <field number="220" name="BenchmarkCurveCurrency" type="CURRENCY" />
    <field number="221" name="BenchmarkCurveName" type="STRING" />
    <field number="222" name="BenchmarkCurvePoint" type="STRING" />
    <field number="223" name="CouponRate" type="PERCENTAGE" />
    <field number="224" name="CouponPaymentDate" type="UTCDATE" />
    <field number="225" name="IssueDate" type="UTCDATE" />
    <field number="226" name="RepurchaseTerm" type="INT" />
    <field number="227" name="RepurchaseRate" type="PERCENTAGE" />
    <field number="228" name="Factor" type="FLOAT" />
    <field number="229" name="TradeOriginationDate" type="UTCDATE" />
    <field number="230" name="ExDate" type="UTCDATE" />
    <field number="231" name="ContractMultiplier" type="FLOAT" />
    <field number="232" name="NoStipulations" type="NUMINGROUP" />
    <field number="233" name="StipulationType" type="STRING" />
    <field number="234" name="StipulationValue" type="STRING" />
    <field number="235" name="YieldType" type="STRING">
      <value enum="AFTERTAX" description="AFTER_TAX_YIELD" />
      <value enum="ANNUAL" description="ANNUAL_YIELD" />
      <value enum="ATISSUE" description="YIELD_AT_ISSUE" />
      <value enum="AVGLIFE" description="YIELD_TO_AVERAGE_LIFE" />
      <value enum="AVGMATURITY" description="YIELD_TO_AVERAGE_MATURITY" />
      <value enum="BOOK" description="BOOK_YIELD" />
      <value enum="CALL" description="YIELD_TO_NEXT_CALL" />
      <value enum="CHANGE" description="YIELD_CHANGE_SINCE_CLOSE" />
      <value enum="CLOSE" description="CLOSING_YIELD" />
      <value enum="COMPOUND" description="COMPOUND_YIELD" />
      <value enum="CURRENT" description="CURRENT_YIELD" />
      <value enum="GROSS" description="TRUE_GROSS_YIELD" />
      <value enum="GOVTEQUIV" description="GOVERNMENT_EQUIVALENT_YIELD" />
      <value enum="INFLATION" description="YIELD_WITH_INFLATION_ASSUMPTION" />
      <value enum="INVERSEFLOATER" description="INVERSE_FLOATER_BOND_YIELD" />
      <value enum="LASTCLOSE" description="MOST_RECENT_CLOSING_YIELD" />
      <value enum="LASTMONTH" description="CLOSING_YIELD_MOST_RECENT_MONTH" />
      <value enum="LASTQUARTER" description="CLOSING_YIELD_MOST_RECENT_QUARTER" />
      <value enum="LASTYEAR" description="CLOSING_YIELD_MOST_RECENT_YEAR" />
      <value enum="LONGAVGLIFE" description="YIELD_TO_LONGEST_AVERAGE_LIFE" />
      <value enum="LONGEST" description="YIELD_TO_LONGEST_AVERAGE" />
      <value enum="MARK" description="MARK_TO_MARKET_YIELD" />
      <value enum="MATURITY" description="YIELD_TO_MATURITY" />
      <value enum="NEXTREFUND" description="YIELD_TO_NEXT_REFUND" />
      <value enum="OPENAVG" description="OPEN_AVERAGE_YIELD" />
      <value enum="PUT" description="YIELD_TO_NEXT_PUT" />
      <value enum="PREVCLOSE" description="PREVIOUS_CLOSE_YIELD" />
      <value enum="PROCEEDS" description="PROCEEDS_YIELD" />
      <value enum="SEMIANNUAL" description="SEMI_ANNUAL_YIELD" />
      <value enum="SHORTAVGLIFE" description="YIELD_TO_SHORTEST_AVERAGE_LIFE" />
      <value enum="SHORTEST" description="YIELD_TO_SHORTEST_AVERAGE" />
      <value enum="SIMPLE" description="SIMPLE_YIELD" />
      <value enum="TAXEQUIV" description="TAX_EQUIVALENT_YIELD" />
      <value enum="TENDER" description="YIELD_TO_TENDER_DATE" />
      <value enum="TRUE" description="TRUE_YIELD" />
      <value enum="WORST" description="YIELD_TO_WORST_CONVENTION" />
    </field>
    <field number="236" name="Yield" type="PERCENTAGE" />
    <field number="237" name="TotalTakedown" type="AMT" />
    <field number="238" name="Concession" type="AMT" />
    <field number="239" name="RepoCollateralSecurityType" type="INT" />
    <field number="240" name="RedemptionDate" type="UTCDATE" />
    <field number="241" name="UnderlyingCouponPaymentDate" type="UTCDATE" />
    <field number="242" name="UnderlyingIssueDate" type="UTCDATE" />
    <field number="243" name="UnderlyingRepoCollateralSecurityType" type="INT" />
    <field number="244" name="UnderlyingRepurchaseTerm" type="INT" />
    <field number="245" name="UnderlyingRepurchaseRate" type="PERCENTAGE" />
    <field number="246" name="UnderlyingFactor" type="FLOAT" />
    <field number="247" name="UnderlyingRedemptionDate" type="UTCDATE" />
    <field number="248" name="LegCouponPaymentDate" type="UTCDATE" />
    <field number="249" name="LegIssueDate" type="UTCDATE" />
    <field number="250" name="LegRepoCollateralSecurityType" type="INT" />
    <field number="251" name="LegRepurchaseTerm" type="INT" />
    <field number="252" name="LegRepurchaseRate" type="PERCENTAGE" />
    <field number="253" name="LegFactor" type="FLOAT" />
    <field number="254" name="LegRedemptionDate" type="UTCDATE" />
    <field number="255" name="CreditRating" type="STRING" />
    <field number="256" name="UnderlyingCreditRating" type="STRING" />
    <field number="257" name="LegCreditRating" type="STRING" />
    <field number="258" name="TradedFlatSwitch" type="BOOLEAN" />
    <field number="259" name="BasisFeatureDate" type="UTCDATE" />
    <field number="260" name="BasisFeaturePrice" type="PRICE" />
    <field number="262" name="MDReqID" type="STRING" />
    <field number="263" name="SubscriptionRequestType" type="CHAR">
      <value enum="0" description="SNAPSHOT" />
      <value enum="1" description="SNAPSHOT_PLUS_UPDATES" />
      <value enum="2" description="DISABLE_PREVIOUS_SNAPSHOT_PLUS_UPDATE_REQUEST" />
    </field>
    <field number="264" name="MarketDepth" type="INT">
<!--
      Temporarily commented out until we can handle
      N>1 scenario

      <value enum="0" description="FULL_BOOK"/>
      <value enum="1" description="TOP_OF_BOOK"/>
-->
    </field>
    <field number="265" name="MDUpdateType" type="INT">
      <value enum="0" description="FULL_REFRESH" />
      <value enum="1" description="INCREMENTAL_REFRESH" />
    </field>
    <field number="266" name="AggregatedBook" type="BOOLEAN" />
    <field number="267" name="NoMDEntryTypes" type="NUMINGROUP" />
    <field number="268" name="NoMDEntries" type="NUMINGROUP" />
    <field number="269" name="MDEntryType" type="CHAR">
      <value enum="0" description="BID" />
      <value enum="1" description="OFFER" />
      <value enum="2" description="TRADE" />
      <value enum="3" description="INDEX_VALUE" />
      <value enum="4" description="OPENING_PRICE" />
      <value enum="5" description="CLOSING_PRICE" />
      <value enum="6" description="SETTLEMENT_PRICE" />
      <value enum="7" description="TRADING_SESSION_HIGH_PRICE" />
      <value enum="8" description="TRADING_SESSION_LOW_PRICE" />
      <value enum="9" description="TRADING_SESSION_VWAP_PRICE" />
      <value enum="A" description="IMBALANCE" />
    </field>
    <field number="270" name="MDEntryPx" type="PRICE" />
    <field number="271" name="MDEntrySize" type="QTY" />
    <field number="272" name="MDEntryDate" type="UTCDATE" />
    <field number="273" name="MDEntryTime" type="UTCTIMEONLY" />
    <field number="274" name="TickDirection" type="CHAR">
      <value enum="0" description="PLUS_TICK" />
      <value enum="1" description="ZERO_PLUS_TICK" />
      <value enum="2" description="MINUS_TICK" />
      <value enum="3" description="ZERO_MINUS_TICK" />
    </field>
    <field number="275" name="MDMkt" type="EXCHANGE" />
    <field number="276" name="QuoteCondition" type="MULTIPLEVALUESTRING">
      <value enum="A" description="OPEN" />
      <value enum="B" description="CLOSED" />
      <value enum="C" description="EXCHANGE_BEST" />
      <value enum="D" description="CONSOLIDATED_BEST" />
      <value enum="E" description="LOCKED" />
      <value enum="F" description="CROSSED" />
      <value enum="G" description="DEPTH" />
      <value enum="H" description="FAST_TRADING" />
      <value enum="I" description="NON_FIRM" />
    </field>
    <field number="277" name="TradeCondition" type="MULTIPLEVALUESTRING">
      <value enum="A" description="CASH" />
      <value enum="B" description="AVERAGE_PRICE_TRADE" />
      <value enum="C" description="CASH_TRADE" />
      <value enum="D" description="NEXT_DAY" />
      <value enum="E" description="OPENING" />
      <value enum="F" description="INTRADAY_TRADE_DETAIL" />
      <value enum="G" description="RULE_127_TRADE" />
      <value enum="H" description="RULE_155_TRADE" />
      <value enum="I" description="SOLD_LAST" />
      <value enum="J" description="NEXT_DAY_TRADE" />
      <value enum="K" description="OPENED" />
      <value enum="L" description="SELLER" />
      <value enum="M" description="SOLD" />
      <value enum="N" description="STOPPED_STOCK" />
      <value enum="P" description="IMBALANCE_MORE_BUYERS" />
      <value enum="Q" description="IMBALANCE_MORE_SELLERS" />
      <value enum="R" description="OPENING_PRICE" />
    </field>
    <field number="278" name="MDEntryID" type="STRING" />
    <field number="279" name="MDUpdateAction" type="CHAR">
      <value enum="0" description="NEW" />
      <value enum="1" description="CHANGE" />
      <value enum="2" description="DELETE" />
    </field>
    <field number="280" name="MDEntryRefID" type="STRING" />
    <field number="281" name="MDReqRejReason" type="CHAR">
      <value enum="0" description="UNKNOWN_SYMBOL" />
      <value enum="1" description="DUPLICATE_MDREQID" />
      <value enum="2" description="INSUFFICIENT_BANDWIDTH" />
      <value enum="3" description="INSUFFICIENT_PERMISSIONS" />
      <value enum="4" description="UNSUPPORTED_SUBSCRIPTIONREQUESTTYPE" />
      <value enum="5" description="UNSUPPORTED_MARKETDEPTH" />
      <value enum="6" description="UNSUPPORTED_MDUPDATETYPE" />
      <value enum="7" description="UNSUPPORTED_AGGREGATEDBOOK" />
      <value enum="8" description="UNSUPPORTED_MDENTRYTYPE" />
      <value enum="9" description="UNSUPPORTED_TRADINGSESSIONID" />
      <value enum="A" description="UNSUPPORTED_SCOPE" />
      <value enum="B" description="UNSUPPORTED_OPENCLOSESETTLEFLAG" />
      <value enum="C" description="UNSUPPORTED_MDIMPLICITDELETE" />
    </field>
    <field number="282" name="MDEntryOriginator" type="STRING" />
    <field number="283" name="LocationID" type="STRING" />
    <field number="284" name="DeskID" type="STRING" />
    <field number="285" name="DeleteReason" type="CHAR">
      <value enum="0" description="CANCELATION" />
      <value enum="1" description="ERROR" />
    </field>
    <field number="286" name="OpenCloseSettleFlag" type="MULTIPLEVALUESTRING">
      <value enum="0" description="DAILY_OPEN" />
      <value enum="1" description="SESSION_OPEN" />
      <value enum="2" description="DELIVERY_SETTLEMENT_PRICE" />
      <value enum="3" description="EXPECTED_PRICE" />
      <value enum="4" description="PRICE_FROM_PREVIOUS_BUSINESS_DAY" />
    </field>
    <field number="287" name="SellerDays" type="INT" />
    <field number="288" name="MDEntryBuyer" type="STRING" />
    <field number="289" name="MDEntrySeller" type="STRING" />
    <field number="290" name="MDEntryPositionNo" type="INT" />
    <field number="291" name="FinancialStatus" type="MULTIPLEVALUESTRING">
      <value enum="1" description="BANKRUPT" />
      <value enum="2" description="PENDING_DELISTING" />
    </field>
    <field number="292" name="CorporateAction" type="MULTIPLEVALUESTRING">
      <value enum="A" description="EX_DIVIDEND" />
      <value enum="B" description="EX_DISTRIBUTION" />
      <value enum="C" description="EX_RIGHTS" />
      <value enum="D" description="NEW" />
      <value enum="E" description="EX_INTEREST" />
    </field>
    <field number="293" name="DefBidSize" type="QTY" />
    <field number="294" name="DefOfferSize" type="QTY" />
    <field number="295" name="NoQuoteEntries" type="NUMINGROUP" />
    <field number="296" name="NoQuoteSets" type="NUMINGROUP" />
    <field number="297" name="QuoteStatus" type="INT">
      <value enum="0" description="ACCEPTED" />
      <value enum="1" description="CANCELED_FOR_SYMBOL" />
      <value enum="2" description="CANCELED_FOR_SECURITY_TYPE" />
      <value enum="3" description="CANCELED_FOR_UNDERLYING" />
      <value enum="4" description="CANCELED_ALL" />
      <value enum="5" description="REJECTED" />
      <value enum="6" description="REMOVED_FROM_MARKET" />
      <value enum="7" description="EXPIRED" />
      <value enum="8" description="QUERY" />
      <value enum="9" description="QUOTE_NOT_FOUND" />
      <value enum="10" description="PENDING" />
    </field>
    <field number="298" name="QuoteCancelType" type="INT" />
    <field number="299" name="QuoteEntryID" type="STRING" />
    <field number="300" name="QuoteRejectReason" type="INT" />
    <field number="301" name="QuoteResponseLevel" type="INT" />
    <field number="302" name="QuoteSetID" type="STRING" />
    <field number="303" name="QuoteRequestType" type="INT">
      <value enum="1" description="MANUAL" />
      <value enum="2" description="AUTOMATIC" />
    </field>
    <field number="304" name="TotQuoteEntries" type="INT" />
    <field number="305" name="UnderlyingSecurityIDSource" type="STRING" />
    <field number="306" name="UnderlyingIssuer" type="STRING" />
    <field number="307" name="UnderlyingSecurityDesc" type="STRING" />
    <field number="308" name="UnderlyingSecurityExchange" type="EXCHANGE" />
    <field number="309" name="UnderlyingSecurityID" type="STRING" />
    <field number="310" name="UnderlyingSecurityType" type="STRING" />
    <field number="311" name="UnderlyingSymbol" type="STRING" />
    <field number="312" name="UnderlyingSymbolSfx" type="STRING" />
    <field number="313" name="UnderlyingMaturityMonthYear" type="MONTHYEAR" />
    <field number="314" name="UnderlyingMaturityDay" type="DAYOFMONTH" />
    <field number="315" name="UnderlyingPutOrCall" type="INT" />
    <field number="316" name="UnderlyingStrikePrice" type="PRICE" />
    <field number="317" name="UnderlyingOptAttribute" type="CHAR" />
    <field number="318" name="UnderlyingCurrency" type="CURRENCY" />
    <field number="319" name="RatioQty" type="QTY" />
    <field number="320" name="SecurityReqID" type="STRING" />
    <field number="321" name="SecurityRequestType" type="INT">
      <value enum="0" description="REQUEST_SECURITY_IDENTITY_AND_SPECIFICATIONS" />
      <value enum="1" description="REQUEST_SECURITY_IDENTITY_FOR_THE_SPECIFICATIONS_PROVIDED" />
      <value enum="2" description="REQUEST_LIST_SECURITY_TYPES" />
      <value enum="3" description="REQUEST_LIST_SECURITIES" />
    </field>
    <field number="322" name="SecurityResponseID" type="STRING" />
    <field number="323" name="SecurityResponseType" type="INT">
      <value enum="1" description="ACCEPT_SECURITY_PROPOSAL_AS_IS" />
      <value enum="2" description="ACCEPT_SECURITY_PROPOSAL_WITH_REVISIONS_AS_INDICATED_IN_THE_MESSAGE" />
      <value enum="3" description="LIST_OF_SECURITY_TYPES_RETURNED_PER_REQUEST" />
      <value enum="4" description="LIST_OF_SECURITIES_RETURNED_PER_REQUEST" />
      <value enum="5" description="REJECT_SECURITY_PROPOSAL" />
      <value enum="6" description="CAN_NOT_MATCH_SELECTION_CRITERIA" />
    </field>
    <field number="324" name="SecurityStatusReqID" type="STRING" />
    <field number="325" name="UnsolicitedIndicator" type="BOOLEAN" />
    <field number="326" name="SecurityTradingStatus" type="INT">
      <value enum="1" description="OPENING_DELAY" />
      <value enum="2" description="TRADING_HALT" />
      <value enum="3" description="RESUME" />
      <value enum="4" description="NO_OPEN" />
      <value enum="5" description="PRICE_INDICATION" />
      <value enum="6" description="TRADING_RANGE_INDICATION" />
      <value enum="7" description="MARKET_IMBALANCE_BUY" />
      <value enum="8" description="MARKET_IMBALANCE_SELL" />
      <value enum="9" description="MARKET_ON_CLOSE_IMBALANCE_BUY" />
      <value enum="10" description="MARKET_ON_CLOSE_IMBALANCE_SELL" />
      <value enum="12" description="NO_MARKET_IMBALANCE" />
      <value enum="13" description="NO_MARKET_ON_CLOSE_IMBALANCE" />
      <value enum="14" description="ITS_PRE_OPENING" />
      <value enum="15" description="NEW_PRICE_INDICATION" />
      <value enum="16" description="TRADE_DISSEMINATION_TIME" />
      <value enum="17" description="READY_TO_TRADE" />
      <value enum="18" description="NOT_AVAILABLE_FOR_TRADING" />
      <value enum="19" description="NOT_TRADED_ON_THIS_MARKET" />
      <value enum="20" description="UNKNOWN_OR_INVALID" />
      <value enum="21" description="PRE_OPEN" />
      <value enum="22" description="OPENING_ROTATION" />
      <value enum="23" description="FAST_MARKET" />
    </field>
    <field number="327" name="HaltReason" type="CHAR">
      <value enum="I" description="ORDER_IMBALANCE" />
      <value enum="X" description="EQUIPMENT_CHANGEOVER" />
      <value enum="P" description="NEWS_PENDING" />
      <value enum="D" description="NEWS_DISSEMINATION" />
      <value enum="E" description="ORDER_INFLUX" />
      <value enum="M" description="ADDITIONAL_INFORMATION" />
    </field>
    <field number="328" name="InViewOfCommon" type="BOOLEAN" />
    <field number="329" name="DueToRelated" type="BOOLEAN" />
    <field number="330" name="BuyVolume" type="QTY" />
    <field number="331" name="SellVolume" type="QTY" />
    <field number="332" name="HighPx" type="PRICE" />
    <field number="333" name="LowPx" type="PRICE" />
    <field number="334" name="Adjustment" type="INT">
      <value enum="1" description="CANCEL" />
      <value enum="2" description="ERROR" />
      <value enum="3" description="CORRECTION" />
    </field>
    <field number="335" name="TradSesReqID" type="STRING" />
    <field number="336" name="TradingSessionID" type="STRING" />
    <field number="337" name="ContraTrader" type="STRING" />
    <field number="338" name="TradSesMethod" type="INT">
      <value enum="1" description="ELECTRONIC" />
      <value enum="2" description="OPEN_OUTCRY" />
      <value enum="3" description="TWO_PARTY" />
    </field>
    <field number="339" name="TradSesMode" type="INT">
      <value enum="1" description="TESTING" />
      <value enum="2" description="SIMULATED" />
      <value enum="3" description="PRODUCTION" />
    </field>
    <field number="340" name="TradSesStatus" type="INT">
      <value enum="0" description="UNKNOWN" />
      <value enum="1" description="HALTED" />
      <value enum="2" description="OPEN" />
      <value enum="3" description="CLOSED" />
      <value enum="4" description="PRE_OPEN" />
      <value enum="5" description="PRE_CLOSE" />
      <value enum="6" description="REQUEST_REJECTED" />
    </field>
    <field number="341" name="TradSesStartTime" type="UTCTIMESTAMP" />
    <field number="342" name="TradSesOpenTime" type="UTCTIMESTAMP" />
    <field number="343" name="TradSesPreCloseTime" type="UTCTIMESTAMP" />
    <field number="344" name="TradSesCloseTime" type="UTCTIMESTAMP" />
    <field number="345" name="TradSesEndTime" type="UTCTIMESTAMP" />
    <field number="346" name="NumberOfOrders" type="INT" />
    <field number="347" name="MessageEncoding" type="STRING" />
    <field number="348" name="EncodedIssuerLen" type="LENGTH" />
    <field number="349" name="EncodedIssuer" type="DATA" />
    <field number="350" name="EncodedSecurityDescLen" type="LENGTH" />
    <field number="351" name="EncodedSecurityDesc" type="DATA" />
    <field number="352" name="EncodedListExecInstLen" type="LENGTH" />
    <field number="353" name="EncodedListExecInst" type="DATA" />
    <field number="354" name="EncodedTextLen" type="LENGTH" />
    <field number="355" name="EncodedText" type="DATA" />
    <field number="356" name="EncodedSubjectLen" type="LENGTH" />
    <field number="357" name="EncodedSubject" type="DATA" />
    <field number="358" name="EncodedHeadlineLen" type="LENGTH" />
    <field number="359" name="EncodedHeadline" type="DATA" />
    <field number="360" name="EncodedAllocTextLen" type="LENGTH" />
    <field number="361" name="EncodedAllocText" type="DATA" />
    <field number="362" name="EncodedUnderlyingIssuerLen" type="LENGTH" />
    <field number="363" name="EncodedUnderlyingIssuer" type="DATA" />
    <field number="364" name="EncodedUnderlyingSecurityDescLen" type="LENGTH" />
    <field number="365" name="EncodedUnderlyingSecurityDesc" type="DATA" />
    <field number="366" name="AllocPrice" type="PRICE" />
    <field number="367" name="QuoteSetValidUntilTime" type="UTCTIMESTAMP" />
    <field number="368" name="QuoteEntryRejectReason" type="INT">
      <value enum="1" description="UNKNOWN_SYMBOL" />
      <value enum="2" description="EXCHANGE" />
      <value enum="3" description="QUOTE_EXCEEDS_LIMIT" />
      <value enum="4" description="TOO_LATE_TO_ENTER" />
      <value enum="5" description="UNKNOWN_QUOTE" />
      <value enum="6" description="DUPLICATE_QUOTE" />
      <value enum="7" description="INVALID_BID" />
      <value enum="8" description="INVALID_PRICE" />
      <value enum="9" description="NOT_AUTHORIZED_TO_QUOTE_SECURITY" />
    </field>
    <field number="369" name="LastMsgSeqNumProcessed" type="SEQNUM" />
    <field number="370" name="OnBehalfOfSendingTime" type="UTCTIMESTAMP" />
    <field number="371" name="RefTagID" type="INT" />
    <field number="372" name="RefMsgType" type="STRING" />
    <field number="373" name="SessionRejectReason" type="INT">
      <value enum="0" description="INVALID_TAG_NUMBER" />
      <value enum="1" description="REQUIRED_TAG_MISSING" />
      <value enum="2" description="TAG_NOT_DEFINED_FOR_THIS_MESSAGE_TYPE" />
      <value enum="3" description="UNDEFINED_TAG" />
      <value enum="4" description="TAG_SPECIFIED_WITHOUT_A_VALUE" />
      <value enum="5" description="VALUE_IS_INCORRECT" />
      <value enum="6" description="INCORRECT_DATA_FORMAT_FOR_VALUE" />
      <value enum="7" description="DECRYPTION_PROBLEM" />
      <value enum="8" description="SIGNATURE_PROBLEM" />
      <value enum="9" description="COMPID_PROBLEM" />
      <value enum="10" description="SENDINGTIME_ACCURACY_PROBLEM" />
      <value enum="11" description="INVALID_MSGTYPE" />
      <value enum="12" description="XML_VALIDATION_ERROR" />
      <value enum="13" description="TAG_APPEARS_MORE_THAN_ONCE" />
      <value enum="14" description="TAG_SPECIFIED_OUT_OF_REQUIRED_ORDER" />
      <value enum="15" description="REPEATING_GROUP_FIELDS_OUT_OF_ORDER" />
      <value enum="16" description="INCORRECT_NUMINGROUP_COUNT_FOR_REPEATING_GROUP" />
      <value enum="17" description="NON_DATA_VALUE_INCLUDES_FIELD_DELIMITER" />
    </field>
    <field number="374" name="BidRequestTransType" type="CHAR">
      <value enum="N" description="NEW" />
      <value enum="C" description="CANCEL" />
    </field>
    <field number="375" name="ContraBroker" type="STRING" />
    <field number="376" name="ComplianceID" type="STRING" />
    <field number="377" name="SolicitedFlag" type="BOOLEAN" />
    <field number="378" name="ExecRestatementReason" type="INT">
      <value enum="0" description="GT_CORPORATE_ACTION" />
      <value enum="1" description="GT_RENEWAL" />
      <value enum="2" description="VERBAL_CHANGE" />
      <value enum="3" description="REPRICING_OF_ORDER" />
      <value enum="4" description="BROKER_OPTION" />
      <value enum="5" description="PARTIAL_DECLINE_OF_ORDERQTY" />
      <value enum="6" description="CANCEL_ON_TRADING_HALT" />
      <value enum="7" description="CANCEL_ON_SYSTEM_FAILURE" />
      <value enum="8" description="MARKET" />
    </field>
    <field number="379" name="BusinessRejectRefID" type="STRING" />
    <field number="380" name="BusinessRejectReason" type="INT">
      <value enum="0" description="OTHER" />
      <value enum="1" description="UNKOWN_ID" />
      <value enum="2" description="UNKNOWN_SECURITY" />
      <value enum="3" description="UNSUPPORTED_MESSAGE_TYPE" />
      <value enum="4" description="APPLICATION_NOT_AVAILABLE" />
      <value enum="5" description="CONDITIONALLY_REQUIRED_FIELD_MISSING" />
      <value enum="6" description="NOT_AUTHORIZED" />
      <value enum="7" description="DELIVERTO_FIRM_NOT_AVAILABLE_AT_THIS_TIME" />
    </field>
    <field number="381" name="GrossTradeAmt" type="AMT" />
    <field number="382" name="NoContraBrokers" type="NUMINGROUP" />
    <field number="383" name="MaxMessageSize" type="LENGTH" />
    <field number="384" name="NoMsgTypes" type="NUMINGROUP" />
    <field number="385" name="MsgDirection" type="CHAR">
      <value enum="S" description="SEND" />
      <value enum="R" description="RECEIVE" />
    </field>
    <field number="386" name="NoTradingSessions" type="NUMINGROUP" />
    <field number="387" name="TotalVolumeTraded" type="QTY" />
    <field number="388" name="DiscretionInst" type="CHAR">
      <value enum="0" description="RELATED_TO_DISPLAYED_PRICE" />
      <value enum="1" description="RELATED_TO_MARKET_PRICE" />
      <value enum="2" description="RELATED_TO_PRIMARY_PRICE" />
      <value enum="3" description="RELATED_TO_LOCAL_PRIMARY_PRICE" />
      <value enum="4" description="RELATED_TO_MIDPOINT_PRICE" />
      <value enum="5" description="RELATED_TO_LAST_TRADE_PRICE" />
    </field>
    <field number="389" name="DiscretionOffset" type="PRICEOFFSET" />
    <field number="390" name="BidID" type="STRING" />
    <field number="391" name="ClientBidID" type="STRING" />
    <field number="392" name="ListName" type="STRING" />
    <field number="393" name="TotalNumSecurities" type="INT" />
    <field number="394" name="BidType" type="INT">
      <value enum="1" description="NON_DISCLOSED_STYLE" />
      <value enum="2" description="DISCLOSED_STYLE" />
      <value enum="3" description="NO_BIDDING_PROCESS" />
    </field>
    <field number="395" name="NumTickets" type="INT" />
    <field type="AMT" name="SideValue1" number="396" />
    <field type="AMT" name="SideValue2" number="397" />
    <field number="398" name="NoBidDescriptors" type="NUMINGROUP" />
    <field number="399" name="BidDescriptorType" type="INT">
      <value enum="1" description="SECTOR" />
      <value enum="2" description="COUNTRY" />
      <value enum="3" description="INDEX" />
    </field>
    <field number="400" name="BidDescriptor" type="STRING" />
    <field number="401" name="SideValueInd" type="INT">
      <value description="SIDEVALUE1" enum="1" />
      <value description="SIDEVALUE_2" enum="2" />
    </field>
    <field number="402" name="LiquidityPctLow" type="PERCENTAGE" />
    <field number="403" name="LiquidityPctHigh" type="PERCENTAGE" />
    <field number="404" name="LiquidityValue" type="AMT" />
    <field number="405" name="EFPTrackingError" type="PERCENTAGE" />
    <field number="406" name="FairValue" type="AMT" />
    <field number="407" name="OutsideIndexPct" type="PERCENTAGE" />
    <field number="408" name="ValueOfFutures" type="AMT" />
    <field number="409" name="LiquidityIndType" type="INT">
      <value enum="1" description="FIVEDAY_MOVING_AVERAGE" />
      <value enum="2" description="TWENTYDAY_MOVING_AVERAGE" />
      <value enum="3" description="NORMAL_MARKET_SIZE" />
      <value enum="4" description="OTHER" />
    </field>
    <field number="410" name="WtAverageLiquidity" type="PERCENTAGE" />
    <field number="411" name="ExchangeForPhysical" type="BOOLEAN" />
    <field number="412" name="OutMainCntryUIndex" type="AMT" />
    <field number="413" name="CrossPercent" type="PERCENTAGE" />
    <field number="414" name="ProgRptReqs" type="INT">
      <value enum="1" description="BUYSIDE_EXPLICITLY_REQUESTS_STATUS_USING_STATUSREQUEST" />
      <value enum="2" description="SELLSIDE_PERIODICALLY_SENDS_STATUS_USING_LISTSTATUS" />
      <value enum="3" description="REAL_TIME_EXECUTION_REPORTS" />
    </field>
    <field number="415" name="ProgPeriodInterval" type="INT" />
    <field number="416" name="IncTaxInd" type="INT">
      <value enum="1" description="NET" />
      <value enum="2" description="GROSS" />
    </field>
    <field number="417" name="NumBidders" type="INT" />
    <field number="418" name="TradeType" type="CHAR" />
    <field number="419" name="BasisPxType" type="CHAR">
      <value enum="2" description="CLOSING_PRICE_AT_MORNING_SESSION" />
      <value enum="3" description="CLOSING_PRICE" />
      <value enum="4" description="CURRENT_PRICE" />
      <value enum="5" description="SQ" />
      <value enum="6" description="VWAP_THROUGH_A_DAY" />
      <value enum="7" description="VWAP_THROUGH_A_MORNING_SESSION" />
      <value enum="8" description="VWAP_THROUGH_AN_AFTERNOON_SESSION" />
      <value enum="9" description="VWAP_THROUGH_A_DAY_EXCEPT_YORI" />
      <value enum="A" description="VWAP_THROUGH_A_MORNING_SESSION_EXCEPT_YORI" />
      <value enum="B" description="VWAP_THROUGH_AN_AFTERNOON_SESSION_EXCEPT_YORI" />
      <value enum="C" description="STRIKE" />
      <value enum="D" description="OPEN" />
      <value enum="Z" description="OTHERS" />
    </field>
    <field number="420" name="NoBidComponents" type="NUMINGROUP" />
    <field number="421" name="Country" type="COUNTRY" />
    <field number="422" name="TotNoStrikes" type="INT" />
    <field number="423" name="PriceType" type="INT">
      <value enum="1" description="PERCENTAGE" />
      <value enum="2" description="PER_SHARE" />
      <value enum="3" description="FIXED_AMOUNT" />
      <value enum="4" description="DISCOUNT" />
      <value enum="5" description="PREMIUM" />
      <value enum="6" description="BASIS_POINTS_RELATIVE_TO_BENCHMARK" />
      <value enum="7" description="TED_PRICE" />
      <value enum="8" description="TED_YIELD" />
    </field>
    <field number="424" name="DayOrderQty" type="QTY" />
    <field number="425" name="DayCumQty" type="QTY" />
    <field number="426" name="DayAvgPx" type="PRICE" />
    <field number="427" name="GTBookingInst" type="INT">
      <value enum="0" description="BOOK_OUT_ALL_TRADES_ON_DAY_OF_EXECUTION" />
      <value enum="1" description="ACCUMULATE_EXECUTIONS_UNTIL_ORDER_IS_FILLED_OR_EXPIRES" />
      <value enum="2" description="ACCUMULATE_UNTIL_VERBALLY_NOTIFIED_OTHERWISE" />
    </field>
    <field number="428" name="NoStrikes" type="NUMINGROUP" />
    <field number="429" name="ListStatusType" type="INT">
      <value enum="1" description="ACK" />
      <value enum="2" description="RESPONSE" />
      <value enum="3" description="TIMED" />
      <value enum="4" description="EXECSTARTED" />
      <value enum="5" description="ALLDONE" />
      <value enum="6" description="ALERT" />
    </field>
    <field number="430" name="NetGrossInd" type="INT">
      <value enum="1" description="NET" />
      <value enum="2" description="GROSS" />
    </field>
    <field number="431" name="ListOrderStatus" type="INT">
      <value enum="1" description="INBIDDINGPROCESS" />
      <value enum="2" description="RECEIVEDFOREXECUTION" />
      <value enum="3" description="EXECUTING" />
      <value enum="4" description="CANCELING" />
      <value enum="5" description="ALERT" />
      <value enum="6" description="ALL_DONE" />
      <value enum="7" description="REJECT" />
    </field>
    <field number="432" name="ExpireDate" type="LOCALMKTDATE" />
    <field number="433" name="ListExecInstType" type="CHAR">
      <value enum="1" description="IMMEDIATE" />
      <value enum="2" description="WAIT_FOR_EXECUTE_INSTRUCTION" />
      <value enum="3" description="EXCHANGE_SWITCH_CIV_ORDER_SELL_DRIVEN" />
      <value enum="4" description="EXCHANGE_SWITCH_CIV_ORDER_BUY_DRIVEN_CASH_TOP_UP" />
      <value enum="5" description="EXCHANGE_SWITCH_CIV_ORDER_BUY_DRIVEN_CASH_WITHDRAW" />
    </field>
    <field number="434" name="CxlRejResponseTo" type="CHAR">
      <value enum="1" description="ORDER_CANCEL_REQUEST" />
      <value enum="2" description="ORDER_CANCEL_REPLACE_REQUEST" />
    </field>
    <field number="435" name="UnderlyingCouponRate" type="PERCENTAGE" />
    <field number="436" name="UnderlyingContractMultiplier" type="FLOAT" />
    <field number="437" name="ContraTradeQty" type="QTY" />
    <field number="438" name="ContraTradeTime" type="UTCTIMESTAMP" />
    <field number="439" name="ClearingFirm" type="STRING" />
    <field number="440" name="ClearingAccount" type="STRING" />
    <field number="441" name="LiquidityNumSecurities" type="INT" />
    <field number="442" name="MultiLegReportingType" type="CHAR" />
    <field number="443" name="StrikeTime" type="UTCTIMESTAMP" />
    <field number="444" name="ListStatusText" type="STRING" />
    <field number="445" name="EncodedListStatusTextLen" type="LENGTH" />
    <field number="446" name="EncodedListStatusText" type="DATA" />
    <field number="447" name="PartyIDSource" type="CHAR">
      <value enum="B" description="BIC" />
      <value enum="C" description="GENERALLY_ACCEPTED_MARKET_PARTICIPANT_IDENTIFIER" />
      <value enum="D" description="PROPRIETARY" />
      <value enum="E" description="ISO_COUNTRY_CODE" />
      <value enum="F" description="SETTLEMENT_ENTITY_LOCATION" />
      <value enum="1" description="KOREAN_INVESTOR_ID" />
      <value enum="2" description="TAIWANESE_QUALIFIED_FOREIGN_INVESTOR_ID_QFII" />
      <value enum="3" description="TAIWANESE_TRADING_ACCOUNT" />
      <value enum="4" description="MALAYSIAN_CENTRAL_DEPOSITORY" />
      <value enum="5" description="CHINESE_B_SHARE" />
      <value enum="6" description="UK_NATIONAL_INSURANCE_OR_PENSION_NUMBER" />
      <value enum="7" description="US_SOCIAL_SECURITY_NUMBER" />
      <value enum="8" description="US_EMPLOYER_IDENTIFICATION_NUMBER" />
      <value enum="9" description="AUSTRALIAN_BUSINESS_NUMBER" />
      <value enum="A" description="AUSTRALIAN_TAX_FILE_NUMBER" />
    </field>
    <field number="448" name="PartyID" type="STRING" />
    <field number="449" name="TotalVolumeTradedDate" type="UTCDATE" />
    <field number="450" name="TotalVolumeTradedTime" type="UTCTIMEONLY" />
    <field number="451" name="NetChgPrevDay" type="PRICEOFFSET" />
    <field number="452" name="PartyRole" type="INT">
      <value enum="1" description="EXECUTING_FIRM" />
      <value enum="2" description="BROKER_OF_CREDIT" />
      <value enum="3" description="CLIENT_ID" />
      <value enum="4" description="CLEARING_FIRM" />
      <value enum="5" description="INVESTOR_ID" />
      <value enum="6" description="INTRODUCING_FIRM" />
      <value enum="7" description="ENTERING_FIRM" />
      <value enum="8" description="LOCATE" />
      <value enum="9" description="FUND_MANAGER_CLIENT_ID" />
      <value enum="10" description="SETTLEMENT_LOCATION" />
      <value enum="11" description="ORDER_ORIGINATION_TRADER" />
      <value enum="12" description="EXECUTING_TRADER" />
      <value enum="13" description="ORDER_ORIGINATION_FIRM" />
      <value enum="14" description="GIVEUP_CLEARING_FIRM" />
      <value enum="15" description="CORRESPONDANT_CLEARING_FIRM" />
      <value enum="16" description="EXECUTING_SYSTEM" />
      <value enum="17" description="CONTRA_FIRM" />
      <value enum="18" description="CONTRA_CLEARING_FIRM" />
      <value enum="19" description="SPONSORING_FIRM" />
      <value enum="20" description="UNDERLYING_CONTRA_FIRM" />
    </field>
    <field number="453" name="NoPartyIDs" type="NUMINGROUP" />
    <field number="454" name="NoSecurityAltID" type="NUMINGROUP" />
    <field number="455" name="SecurityAltID" type="STRING" />
    <field number="456" name="SecurityAltIDSource" type="STRING" />
    <field number="457" name="NoUnderlyingSecurityAltID" type="NUMINGROUP" />
    <field number="458" name="UnderlyingSecurityAltID" type="STRING" />
    <field number="459" name="UnderlyingSecurityAltIDSource" type="STRING" />
    <field number="460" name="Product" type="INT">
      <value enum="1" description="AGENCY" />
      <value enum="2" description="COMMODITY" />
      <value enum="3" description="CORPORATE" />
      <value enum="4" description="CURRENCY" />
      <value enum="5" description="EQUITY" />
      <value enum="6" description="GOVERNMENT" />
      <value enum="7" description="INDEX" />
      <value enum="8" description="LOAN" />
      <value enum="9" description="MONEYMARKET" />
      <value enum="10" description="MORTGAGE" />
      <value enum="11" description="MUNICIPAL" />
      <value enum="12" description="OTHER" />
    </field>
    <field number="461" name="CFICode" type="STRING" />
    <field number="462" name="UnderlyingProduct" type="INT" />
    <field number="463" name="UnderlyingCFICode" type="STRING" />
    <field number="464" name="TestMessageIndicator" type="BOOLEAN" />
    <field number="465" name="QuantityType" type="INT">
      <value enum="1" description="SHARES" />
      <value enum="2" description="BONDS" />
      <value enum="3" description="CURRENTFACE" />
      <value enum="4" description="ORIGINALFACE" />
      <value enum="5" description="CURRENCY" />
      <value enum="6" description="CONTRACTS" />
      <value enum="7" description="OTHER" />
      <value enum="8" description="PAR" />
    </field>
    <field number="466" name="BookingRefID" type="STRING" />
    <field number="467" name="IndividualAllocID" type="STRING" />
    <field number="468" name="RoundingDirection" type="CHAR" />
    <field number="469" name="RoundingModulus" type="FLOAT" />
    <field number="470" name="CountryOfIssue" type="COUNTRY" />
    <field number="471" name="StateOrProvinceOfIssue" type="STRING" />
    <field number="472" name="LocaleOfIssue" type="STRING" />
    <field number="473" name="NoRegistDtls" type="NUMINGROUP" />
    <field number="474" name="MailingDtls" type="STRING" />
    <field number="475" name="InvestorCountryOfResidence" type="COUNTRY" />
    <field number="476" name="PaymentRef" type="STRING" />
    <field number="477" name="DistribPaymentMethod" type="INT" />
    <field number="478" name="CashDistribCurr" type="CURRENCY" />
    <field number="479" name="CommCurrency" type="CURRENCY" />
    <field number="480" name="CancellationRights" type="CHAR" />
    <field number="481" name="MoneyLaunderingStatus" type="CHAR">
      <value enum="Y" description="PASSED" />
      <value enum="N" description="NOT_CHECKED" />
      <value enum="1" description="EXEMPT_BELOW_THE_LIMIT" />
      <value enum="2" description="EXEMPT_CLIENT_MONEY_TYPE_EXEMPTION" />
      <value enum="3" description="EXEMPT_AUTHORISED_CREDIT_OR_FINANCIAL_INSTITUTION" />
    </field>
    <field number="482" name="MailingInst" type="STRING" />
    <field number="483" name="TransBkdTime" type="UTCTIMESTAMP" />
    <field number="484" name="ExecPriceType" type="CHAR" />
    <field number="485" name="ExecPriceAdjustment" type="FLOAT" />
    <field number="486" name="DateOfBirth" type="LOCALMKTDATE" />
    <field number="487" name="TradeReportTransType" type="CHAR">
      <value enum="N" description="NEW" />
      <value enum="C" description="CANCEL" />
      <value enum="R" description="REPLACE" />
    </field>
    <field number="488" name="CardHolderName" type="STRING" />
    <field number="489" name="CardNumber" type="STRING" />
    <field number="490" name="CardExpDate" type="LOCALMKTDATE" />
    <field number="491" name="CardIssNo" type="STRING" />
    <field number="492" name="PaymentMethod" type="INT" />
    <field number="493" name="RegistAcctType" type="STRING" />
    <field number="494" name="Designation" type="STRING" />
    <field number="495" name="TaxAdvantageType" type="INT" />
    <field number="496" name="RegistRejReasonText" type="STRING" />
    <field number="497" name="FundRenewWaiv" type="CHAR" />
    <field number="498" name="CashDistribAgentName" type="STRING" />
    <field number="499" name="CashDistribAgentCode" type="STRING" />
    <field number="500" name="CashDistribAgentAcctNumber" type="STRING" />
    <field number="501" name="CashDistribPayRef" type="STRING" />
    <field number="502" name="CashDistribAgentAcctName" type="STRING" />
    <field number="503" name="CardStartDate" type="LOCALMKTDATE" />
    <field number="504" name="PaymentDate" type="LOCALMKTDATE" />
    <field number="505" name="PaymentRemitterID" type="STRING" />
    <field number="506" name="RegistStatus" type="CHAR" />
    <field number="507" name="RegistRejReasonCode" type="INT" />
    <field number="508" name="RegistRefID" type="STRING" />
    <field number="509" name="RegistDetls" type="STRING" />
    <field number="510" name="NoDistribInsts" type="NUMINGROUP" />
    <field number="511" name="RegistEmail" type="STRING" />
    <field number="512" name="DistribPercentage" type="PERCENTAGE" />
    <field number="513" name="RegistID" type="STRING" />
    <field number="514" name="RegistTransType" type="CHAR">
      <value enum="0" description="NEW" />
      <value enum="1" description="REPLACE" />
      <value enum="2" description="CANCEL" />
    </field>
    <field number="515" name="ExecValuationPoint" type="UTCTIMESTAMP" />
    <field number="516" name="OrderPercent" type="PERCENTAGE" />
    <field number="517" name="OwnershipType" type="CHAR" />
    <field number="518" name="NoContAmts" type="NUMINGROUP" />
    <field number="519" name="ContAmtType" type="INT" />
    <field number="520" name="ContAmtValue" type="FLOAT" />
    <field number="521" name="ContAmtCurr" type="CURRENCY" />
    <field number="522" name="OwnerType" type="INT">
      <value enum="1" description="INDIVIDUAL_INVESTOR" />
      <value enum="2" description="PUBLIC_COMPANY" />
      <value enum="3" description="PRIVATE_COMPANY" />
      <value enum="4" description="INDIVIDUAL_TRUSTEE" />
      <value enum="5" description="COMPANY_TRUSTEE" />
      <value enum="6" description="PENSION_PLAN" />
      <value enum="7" description="CUSTODIAN_UNDER_GIFTS_TO_MINORS_ACT" />
      <value enum="8" description="TRUSTS" />
      <value enum="9" description="FIDUCIARIES" />
      <value enum="10" description="NETWORKING_SUB_ACCOUNT" />
      <value enum="11" description="NON_PROFIT_ORGANIZATION" />
      <value enum="12" description="CORPORATE_BODY" />
      <value enum="13" description="NOMINEE" />
    </field>
    <field number="523" name="PartySubID" type="STRING" />
    <field number="524" name="NestedPartyID" type="STRING" />
    <field number="525" name="NestedPartyIDSource" type="CHAR" />
    <field number="526" name="SecondaryClOrdID" type="STRING" />
    <field number="527" name="SecondaryExecID" type="STRING" />
    <field number="528" name="OrderCapacity" type="CHAR">
      <value enum="A" description="AGENCY" />
      <value enum="G" description="PROPRIETARY" />
      <value enum="I" description="INDIVIDUAL" />
      <value enum="P" description="PRINCIPAL" />
      <value enum="R" description="RISKLESS_PRINCIPAL" />
      <value enum="W" description="AGENT_FOR_OTHER_MEMBER" />
    </field>
    <field number="529" name="OrderRestrictions" type="MULTIPLEVALUESTRING">
      <value enum="1" description="PROGRAM_TRADE" />
      <value enum="2" description="INDEX_ARBITRAGE" />
      <value enum="3" description="NON_INDEX_ARBITRAGE" />
      <value enum="4" description="COMPETING_MARKET_MAKER" />
      <value enum="5" description="ACTING_AS_MARKET_MAKER_OR_SPECIALIST_IN_THE_SECURITY" />
      <value enum="6" description="ACTING_AS_MARKET_MAKER_OR_SPECIALIST_IN_THE_UNDERLYING_SECURITY_OF_A_DERIVATIVE_SECURITY" />
      <value enum="7" description="FOREIGN_ENTITY" />
      <value enum="8" description="EXTERNAL_MARKET_PARTICIPANT" />
      <value enum="9" description="EXTERNAL_INTER_CONNECTED_MARKET_LINKAGE" />
      <value enum="A" description="RISKLESS_ARBITRAGE" />
    </field>
    <field number="530" name="MassCancelRequestType" type="CHAR">
      <value enum="1" description="CANCEL_ORDERS_FOR_A_SECURITY" />
      <value enum="2" description="CANCEL_ORDERS_FOR_AN_UNDERLYING_SECURITY" />
      <value enum="3" description="CANCEL_ORDERS_FOR_A_PRODUCT" />
      <value enum="4" description="CANCEL_ORDERS_FOR_A_CFICODE" />
      <value enum="5" description="CANCEL_ORDERS_FOR_A_SECURITYTYPE" />
      <value enum="6" description="CANCEL_ORDERS_FOR_A_TRADING_SESSION" />
      <value enum="7" description="CANCEL_ALL_ORDERS" />
    </field>
    <field number="531" name="MassCancelResponse" type="CHAR">
      <value enum="0" description="CANCEL_REQUEST_REJECTED" />
      <value enum="1" description="CANCEL_ORDERS_FOR_A_SECURITY" />
      <value enum="2" description="CANCEL_ORDERS_FOR_AN_UNDERLYING_SECURITY" />
      <value enum="3" description="CANCEL_ORDERS_FOR_A_PRODUCT" />
      <value enum="4" description="CANCEL_ORDERS_FOR_A_CFICODE" />
      <value enum="5" description="CANCEL_ORDERS_FOR_A_SECURITYTYPE" />
      <value enum="6" description="CANCEL_ORDERS_FOR_A_TRADING_SESSION" />
      <value enum="7" description="CANCEL_ALL_ORDERS" />
    </field>
    <field number="532" name="MassCancelRejectReason" type="CHAR" />
    <field number="533" name="TotalAffectedOrders" type="INT" />
    <field number="534" name="NoAffectedOrders" type="INT" />
    <field number="535" name="AffectedOrderID" type="STRING" />
    <field number="536" name="AffectedSecondaryOrderID" type="STRING" />
    <field number="537" name="QuoteType" type="INT">
      <value enum="0" description="INDICATIVE" />
      <value enum="1" description="TRADEABLE" />
      <value enum="2" description="RESTRICTED_TRADEABLE" />
    </field>
    <field number="538" name="NestedPartyRole" type="INT" />
    <field number="539" name="NoNestedPartyIDs" type="NUMINGROUP" />
    <field number="540" name="TotalAccruedInterestAmt" type="AMT" />
    <field number="541" name="MaturityDate" type="LOCALMKTDATE" />
    <field number="542" name="UnderlyingMaturityDate" type="LOCALMKTDATE" />
    <field number="543" name="InstrRegistry" type="STRING">
      <value enum="Code" description="COUNTRY_IN_WHICH_REGISTRY_IS_KEPT" />
      <value enum="ZZ" description="PHYSICAL_OR_BEARER" />
    </field>
    <field number="544" name="CashMargin" type="CHAR">
      <value enum="1" description="CASH" />
      <value enum="2" description="MARGIN_OPEN" />
      <value enum="3" description="MARGIN_CLOSE" />
    </field>
    <field number="545" name="NestedPartySubID" type="STRING" />
    <field number="546" name="Scope" type="MULTIPLEVALUESTRING">
      <value enum="1" description="LOCAL" />
      <value enum="2" description="NATIONAL" />
      <value enum="3" description="GLOBAL" />
    </field>
    <field number="547" name="MDImplicitDelete" type="BOOLEAN" />
    <field number="548" name="CrossID" type="STRING" />
    <field number="549" name="CrossType" type="INT">
      <value enum="1" description="CROSS_TRADE_WHICH_IS_EXECUTED_COMPLETELY_OR_NOT" />
      <value enum="2" description="CROSS_TRADE_WHICH_IS_EXECUTED_PARTIALLY_AND_THE_REST_IS_CANCELLED" />
      <value enum="3" description="CROSS_TRADE_WHICH_IS_PARTIALLY_EXECUTED_WITH_THE_UNFILLED_PORTIONS_REMAINING_ACTIVE" />
      <value enum="4" description="CROSS_TRADE_IS_EXECUTED_WITH_EXISTING_ORDERS_WITH_THE_SAME_PRICE" />
    </field>
    <field number="550" name="CrossPrioritization" type="INT" />
    <field number="551" name="OrigCrossID" type="STRING" />
    <field number="552" name="NoSides" type="NUMINGROUP">
      <value enum="1" description="ONE_SIDE" />
      <value enum="2" description="BOTH_SIDES" />
    </field>
    <field number="553" name="Username" type="STRING" />
    <field number="554" name="Password" type="STRING" />
    <field number="555" name="NoLegs" type="NUMINGROUP" />
    <field number="556" name="LegCurrency" type="CURRENCY" />
    <field number="557" name="TotalNumSecurityTypes" type="INT" />
    <field number="558" name="NoSecurityTypes" type="NUMINGROUP" />
    <field number="559" name="SecurityListRequestType" type="INT">
      <value enum="0" description="SYMBOL" />
      <value enum="1" description="SECURITYTYPE_AND" />
      <value enum="2" description="PRODUCT" />
      <value enum="3" description="TRADINGSESSIONID" />
      <value enum="4" description="ALL_SECURITIES" />
    </field>
    <field number="560" name="SecurityRequestResult" type="INT">
      <value enum="0" description="VALID_REQUEST" />
      <value enum="1" description="INVALID_OR_UNSUPPORTED_REQUEST" />
      <value enum="2" description="NO_INSTRUMENTS_FOUND_THAT_MATCH_SELECTION_CRITERIA" />
      <value enum="3" description="NOT_AUTHORIZED_TO_RETRIEVE_INSTRUMENT_DATA" />
      <value enum="4" description="INSTRUMENT_DATA_TEMPORARILY_UNAVAILABLE" />
      <value enum="5" description="REQUEST_FOR_INSTRUMENT_DATA_NOT_SUPPORTED" />
    </field>
    <field number="561" name="RoundLot" type="QTY" />
    <field number="562" name="MinTradeVol" type="QTY" />
    <field number="563" name="MultiLegRptTypeReq" type="INT" />
    <field number="564" name="LegPositionEffect" type="CHAR" />
    <field number="565" name="LegCoveredOrUncovered" type="INT" />
    <field number="566" name="LegPrice" type="PRICE" />
    <field number="567" name="TradSesStatusRejReason" type="INT">
      <value enum="1" description="UNKNOWN_OR_INVALID_TRADINGSESSIONID" />
    </field>
    <field number="568" name="TradeRequestID" type="STRING" />
    <field number="569" name="TradeRequestType" type="INT">
      <value enum="0" description="ALL_TRADES" />
      <value enum="1" description="MATCHED_TRADES_MATCHING_CRITERIA_PROVIDED_ON_REQUEST" />
      <value enum="2" description="UNMATCHED_TRADES_THAT_MATCH_CRITERIA" />
      <value enum="3" description="UNREPORTED_TRADES_THAT_MATCH_CRITERIA" />
      <value enum="4" description="ADVISORIES_THAT_MATCH_CRITERIA" />
    </field>
    <field number="570" name="PreviouslyReported" type="BOOLEAN" />
    <field number="571" name="TradeReportID" type="STRING" />
    <field number="572" name="TradeReportRefID" type="STRING" />
    <field number="573" name="MatchStatus" type="CHAR">
      <value enum="0" description="COMPARED" />
      <value enum="1" description="UNCOMPARED" />
      <value enum="2" description="ADVISORY_OR_ALERT" />
    </field>
    <field number="574" name="MatchType" type="STRING">
      <value enum="A1" description="EXACT_MATCH_PLUS_FOUR_BADGES_AND_EXECUTION_TIME" />
      <value enum="A2" description="EXACT_MATCH_PLUS_FOUR_BADGES" />
      <value enum="A3" description="EXACT_MATCH_PLUS_TWO_BADGES_AND_EXECUTION_TIME" />
      <value enum="A4" description="EXACT_MATCH_PLUS_TWO_BADGES" />
      <value enum="A5" description="EXACT_MATCH_PLUS_EXECUTION_TIME" />
      <value enum="AQ" description="COMPARED_RECORDS_RESULTING_FROM_STAMPED_ADVISORIES_OR_SPECIALIST_ACCEPTS" />
      <value description="SUMMARIZED_MATCH_USING_A1_TO_A5" enum="S5" />
      <value enum="M1" description="EXACT_MATCH_MINUS_BADGES_AND_EXECUTION_TIME" />
      <value enum="M2" description="SUMMARIZED_MATCH_MINUS_BADGES_AND_TIMES" />
      <value enum="MT" description="OCS_LOCKED_IN" />
      <value description="ACT_M1_MATCH" enum="M1" />
      <value description="ACT_M2_MATCH" enum="M2" />
      <value enum="M3" description="ACT_ACCEPTED_TRADE" />
      <value enum="M4" description="ACT_DEFAULT_TRADE" />
      <value description="ACT_DEFAULT_AFTER_M2" enum="M5" />
      <value description="ACT_M6_MATCH" enum="M6" />
      <value enum="MT" description="NON_ACT" />
    </field>
    <field number="575" name="OddLot" type="BOOLEAN" />
    <field number="576" name="NoClearingInstructions" type="INT" />
    <field number="577" name="ClearingInstruction" type="INT">
      <value enum="0" description="PROCESS_NORMALLY" />
      <value enum="1" description="EXCLUDE_FROM_ALL_NETTING" />
      <value enum="2" description="BILATERAL_NETTING_ONLY" />
      <value enum="3" description="EX_CLEARING" />
      <value enum="4" description="SPECIAL_TRADE" />
      <value enum="5" description="MULTILATERAL_NETTING" />
      <value enum="6" description="CLEAR_AGAINST_CENTRAL_COUNTERPARTY" />
      <value enum="7" description="EXCLUDE_FROM_CENTRAL_COUNTERPARTY" />
      <value enum="8" description="MANUAL_MODE" />
      <value enum="9" description="AUTOMATIC_POSTING_MODE" />
      <value enum="10" description="AUTOMATIC_GIVE_UP_MODE" />
    </field>
    <field number="578" name="TradeInputSource" type="STRING" />
    <field number="579" name="TradeInputDevice" type="STRING" />
    <field number="580" name="NoDates" type="INT" />
    <field number="581" name="AccountType" type="INT">
      <value enum="1" description="ACCOUNT_IS_CARRIED_ON_CUSTOMER_SIDE_OF_BOOKS" />
      <value enum="2" description="ACCOUNT_IS_CARRIED_ON_NON_CUSTOMER_SIDE_OF_BOOKS" />
      <value enum="3" description="HOUSE_TRADER" />
      <value enum="4" description="FLOOR_TRADER" />
      <value enum="6" description="ACCOUNT_IS_CARRIED_ON_NON_CUSTOMER_SIDE_OF_BOOKS_AND_IS_CROSS_MARGINED" />
      <value enum="7" description="ACCOUNT_IS_HOUSE_TRADER_AND_IS_CROSS_MARGINED" />
      <value enum="8" description="JOINT_BACKOFFICE_ACCOUNT" />
    </field>
    <field number="582" name="CustOrderCapacity" type="INT" />
    <field number="583" name="ClOrdLinkID" type="STRING" />
    <field number="584" name="MassStatusReqID" type="STRING" />
    <field number="585" name="MassStatusReqType" type="INT">
      <value enum="1" description="STATUS_FOR_ORDERS_FOR_A_SECURITY" />
      <value enum="2" description="STATUS_FOR_ORDERS_FOR_AN_UNDERLYING_SECURITY" />
      <value enum="3" description="STATUS_FOR_ORDERS_FOR_A_PRODUCT" />
      <value enum="4" description="STATUS_FOR_ORDERS_FOR_A_CFICODE" />
      <value enum="5" description="STATUS_FOR_ORDERS_FOR_A_SECURITYTYPE" />
      <value enum="6" description="STATUS_FOR_ORDERS_FOR_A_TRADING_SESSION" />
      <value enum="7" description="STATUS_FOR_ALL_ORDERS" />
      <value enum="8" description="STATUS_FOR_ORDERS_FOR_A_PARTYID" />
    </field>
    <field number="586" name="OrigOrdModTime" type="UTCTIMESTAMP" />
    <field number="587" name="LegSettlmntTyp" type="CHAR" />
    <field number="588" name="LegFutSettDate" type="LOCALMKTDATE" />
    <field number="589" name="DayBookingInst" type="CHAR" />
    <field number="590" name="BookingUnit" type="CHAR" />
    <field number="591" name="PreallocMethod" type="CHAR" />
    <field number="592" name="UnderlyingCountryOfIssue" type="COUNTRY" />
    <field number="593" name="UnderlyingStateOrProvinceOfIssue" type="STRING" />
    <field number="594" name="UnderlyingLocaleOfIssue" type="STRING" />
    <field number="595" name="UnderlyingInstrRegistry" type="STRING" />
    <field number="596" name="LegCountryOfIssue" type="COUNTRY" />
    <field number="597" name="LegStateOrProvinceOfIssue" type="STRING" />
    <field number="598" name="LegLocaleOfIssue" type="STRING" />
    <field number="599" name="LegInstrRegistry" type="STRING" />
    <field number="600" name="LegSymbol" type="STRING" />
    <field number="601" name="LegSymbolSfx" type="STRING" />
    <field number="602" name="LegSecurityID" type="STRING" />
    <field number="603" name="LegSecurityIDSource" type="STRING" />
    <field number="604" name="NoLegSecurityAltID" type="STRING" />
    <field number="605" name="LegSecurityAltID" type="STRING" />
    <field number="606" name="LegSecurityAltIDSource" type="STRING" />
    <field number="607" name="LegProduct" type="INT" />
    <field number="608" name="LegCFICode" type="STRING" />
    <field number="609" name="LegSecurityType" type="STRING" />
    <field number="610" name="LegMaturityMonthYear" type="MONTHYEAR" />
    <field number="611" name="LegMaturityDate" type="LOCALMKTDATE" />
    <field number="612" name="LegStrikePrice" type="PRICE" />
    <field number="613" name="LegOptAttribute" type="CHAR" />
    <field number="614" name="LegContractMultiplier" type="FLOAT" />
    <field number="615" name="LegCouponRate" type="PERCENTAGE" />
    <field number="616" name="LegSecurityExchange" type="EXCHANGE" />
    <field number="617" name="LegIssuer" type="STRING" />
    <field number="618" name="EncodedLegIssuerLen" type="LENGTH" />
    <field number="619" name="EncodedLegIssuer" type="DATA" />
    <field number="620" name="LegSecurityDesc" type="STRING" />
    <field number="621" name="EncodedLegSecurityDescLen" type="LENGTH" />
    <field number="622" name="EncodedLegSecurityDesc" type="DATA" />
    <field number="623" name="LegRatioQty" type="FLOAT" />
    <field number="624" name="LegSide" type="CHAR" />
    <field number="625" name="TradingSessionSubID" type="STRING" />
    <field number="626" name="AllocType" type="INT">
      <value enum="1" description="BUYSIDE_CALCULATED" />
      <value enum="2" description="BUYSIDE_PRELIMINARY" />
      <value enum="3" description="SELLSIDE_CALCULATED_USING_PRELIMINARY" />
      <value enum="4" description="SELLSIDE_CALCULATED_WITHOUT_PRELIMINARY" />
      <value enum="5" description="BUYSIDE_READY_TO_BOOK_SINGLE_ORDER" />
      <value enum="6" description="BUYSIDE_READY_TO_BOOK_COMBINED_SET_OF_ORDERS" />
    </field>
    <field number="627" name="NoHops" type="NUMINGROUP" />
    <field number="628" name="HopCompID" type="STRING" />
    <field number="629" name="HopSendingTime" type="UTCTIMESTAMP" />
    <field number="630" name="HopRefID" type="SEQNUM" />
    <field number="631" name="MidPx" type="PRICE" />
    <field number="632" name="BidYield" type="PERCENTAGE" />
    <field number="633" name="MidYield" type="PERCENTAGE" />
    <field number="634" name="OfferYield" type="PERCENTAGE" />
    <field number="635" name="ClearingFeeIndicator" type="STRING" />
    <field number="636" name="WorkingIndicator" type="BOOLEAN" />
    <field number="637" name="LegLastPx" type="PRICE" />
    <field number="638" name="PriorityIndicator" type="INT">
      <value enum="0" description="PRIORITY_UNCHANGED" />
      <value enum="1" description="LOST_PRIORITY_AS_RESULT_OF_ORDER_CHANGE" />
    </field>
    <field number="639" name="PriceImprovement" type="PRICEOFFSET" />
    <field type="PRICE" name="Price2" number="640" />
    <field type="PRICEOFFSET" name="LastForwardPoints2" number="641" />
    <field type="PRICEOFFSET" name="BidForwardPoints2" number="642" />
    <field type="PRICEOFFSET" name="OfferForwardPoints2" number="643" />
    <field number="644" name="RFQReqID" type="STRING" />
    <field number="645" name="MktBidPx" type="PRICE" />
    <field number="646" name="MktOfferPx" type="PRICE" />
    <field number="647" name="MinBidSize" type="QTY" />
    <field number="648" name="MinOfferSize" type="QTY" />
    <field number="649" name="QuoteStatusReqID" type="STRING" />
    <field number="650" name="LegalConfirm" type="BOOLEAN" />
    <field number="651" name="UnderlyingLastPx" type="PRICE" />
    <field number="652" name="UnderlyingLastQty" type="QTY" />
    <field number="653" name="SecDefStatus" type="INT">
      <value enum="0" description="PENDING_APPROVAL" />
      <value enum="1" description="APPROVED" />
      <value enum="2" description="REJECTED" />
      <value enum="3" description="UNAUTHORIZED_REQUEST" />
      <value enum="4" description="INVALID_DEFINITION_REQUEST" />
    </field>
    <field number="654" name="LegRefID" type="STRING" />
    <field number="655" name="ContraLegRefID" type="STRING" />
    <field number="656" name="SettlCurrBidFxRate" type="FLOAT" />
    <field number="657" name="SettlCurrOfferFxRate" type="FLOAT" />
    <field number="658" name="QuoteRequestRejectReason" type="INT" />
    <field number="659" name="SideComplianceID" type="STRING" />
	<field number="790" name="OrdStatusReqID" type="STRING" />
	<field number="7541" name="LastPx2" type="PRICE" />
	<field number="7542" name="LastSpotRate2" type="PRICE" />
	<field number="7543" name="LeavesQty2" type="QTY" />
	<field number="7544" name="CumQty2" type="QTY" />
	<field number="7545" name="SettlCurrAmt2" type="AMT"/>
	<field number="7546" name="RequestedSize" type="QTY" />
	<field number="7551" name="BidSize2" type="QTY" />
	<field number="7552" name="OfferSize2" type="QTY" />
	<field number="7553" name="MarketRangeABS" type="PRICE"/>
	<field number="7554" name="AdditionalExecInst" type="STRING"/>
	<field number="7601" name="CoverTxnIds" type="STRING" />
	<field number="7602" name="CoveredTxnIds" type="STRING" />
  </fields>
</fix>
 