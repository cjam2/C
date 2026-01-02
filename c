Windows =
    List.Generate(
        () => RangeStart,
        each _ < RangeEnd,
        each Date.AddDays(_, WindowDays),
        each [
            From = _,
            To =
                if Date.AddDays(_, WindowDays) < RangeEnd
                then Date.AddDays(_, WindowDays)
                else RangeEnd
        ]
    )