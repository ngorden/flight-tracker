open Unix

let usage_msg =
  "tracker -s <start_time> -e <end_time> -f <start_location> -t <end_location>"

let plane_landing = ""
let plane_takeoff = ""
let plane_flying = ""
let start_location = ref ""
let end_location = ref ""
let start_time = ref 0
let end_time = ref 0
let anon_fun unknowns = print_endline unknowns

let speclist =
  [
    ("-f", Arg.Set_string start_location, "The flight origin city");
    ("-t", Arg.Set_string end_location, "The flight destination city");
    ("-s", Arg.Set_int start_time, "The flight departure time (unix timestamp)");
    ("-e", Arg.Set_int end_time, "The flight arrival time (unix timestamp)");
  ]

let get_width =
  let width_opt = Terminal_size.get_columns () in
  match width_opt with None -> 0 | Some width -> width

let replace_str_at_index original index replacement =
  if index < 0 || index >= String.length original then original
  else
    let start_index = index + String.length replacement in
    let end_index =
      String.length original - index - String.length replacement
    in

    let prefix = String.sub original 0 index in
    let suffix = String.sub original start_index end_index in
    prefix ^ replacement ^ suffix

let find_max x y = if x > y then x else y
let find_min x y = if x < y then x else y

let rec main_loop stop output_line =
  if stop == true then Printf.printf "Flight completed!"
  else
    let now = Unix.gettimeofday () in
    let elapsed = Float.of_int end_time.contents -. now in
    let total_trip = Float.of_int (end_time.contents - start_time.contents) in
    let offset = 8 + String.length start_location.contents in
    let complete = 1. -. (elapsed /. total_trip) in
    let replace_string =
      Printf.sprintf "%s (%.2f %%)"
        (if complete <= 0.25 then plane_takeoff
         else if complete >= 0.75 then plane_landing
         else plane_flying)
        (100. *. complete)
    in
    let idx =
      find_min
        (find_max offset (String.length replace_string)
        + Int.of_float
            (Float.of_int
               (String.length output_line
               - (1 + String.length end_location.contents)
               - offset)
            *. (1. -. (elapsed /. total_trip)))
        - String.length end_location.contents
        - 8)
        (String.length output_line - String.length replace_string)
    in
    let line_to_print = replace_str_at_index output_line idx replace_string in
    let () = Printf.printf "\027[2K%s\r%!" line_to_print in
    let () = Unix.sleepf 1. in
    let now = Unix.gettimeofday () in
    main_loop (Int.of_float now >= end_time.contents) output_line

let () =
  Arg.parse speclist anon_fun usage_msg;

  let width = get_width in
  let output_str = String.make width '.' in
  let start_time_tm = Unix.localtime (Float.of_int start_time.contents) in
  let end_time_tm = Unix.localtime (Float.of_int end_time.contents) in
  let start_time_str =
    Printf.sprintf "(%02d:%02d) %s " start_time_tm.tm_hour start_time_tm.tm_min
      start_location.contents
  in
  let end_time_str =
    Printf.sprintf " %s (%02d:%02d)" end_location.contents end_time_tm.tm_hour
      end_time_tm.tm_min
  in
  let output_str = replace_str_at_index output_str 0 start_time_str in
  let output_str =
    replace_str_at_index output_str
      (String.length output_str - String.length end_time_str)
      end_time_str
  in
  main_loop false output_str
