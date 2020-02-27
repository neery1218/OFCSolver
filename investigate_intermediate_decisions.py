import click
import pandas
import requests
import logging
import json


def format_hand(hand):
    # reformat hand to solver api spec
    # "KdKh--AdAcAh" -> Kd Kh//Ad Ac Ah fml
    formatted_hand_rows = []
    for hand_row in hand.split("-"):
        cards = []
        for j in range(0, len(hand_row) // 2):
            cards.append(hand_row[2 * j:2 * j + 2])

        formatted_hand_rows.append(" ".join(cards))

    formatted_hand = "/".join(formatted_hand_rows)
    return formatted_hand


@click.command()
@click.option('--filename', default='sims.txt', help='file which contains list of all sims')
@click.option('--solver-ip', default='localhost', help='IP address of ofcsolver')
def main(filename, solver_ip):
    # requests_log = logging.getLogger("requests.packages.urllib3")
    # requests_log.setLevel(logging.DEBUG)
    # requests_log.propagate = True
    with open(filename, 'r') as f:
        for sim_file in f.readlines():
            print(sim_file)
            df = pandas.read_csv('data/' + sim_file.strip())

            for i in range(0, 3):
                # element is (decision: string, ev: float)
                resolved_decisions = []
                evs = []
                if not (f"Pull_{i}" in df):
                    break

                # sim hand_i with pull_i
                for row in df.itertuples():
                    # hand: "KdKh--AdAcAh"
                    # pull: "Jh Qh 4s"
                    hand = getattr(row, f"Hand_{i}").strip()
                    pull = getattr(row, f"Pull_{i}").strip()
                    formatted_hand = format_hand(hand)

                    # call ofcsolver
                    print("Hand: {}".format(formatted_hand))
                    print("Pull: {}".format(pull))

                    resp = requests.get(
                        "http://" + solver_ip + ":8080/eval",
                        params={
                            'type': 'ultimate',
                            'pull': pull,
                            'my_hand': formatted_hand
                        }
                    )

                    json_resp = json.loads(resp.text)
                    best_decision = json_resp['best']
                    best_decision_ev = json_resp[best_decision]

                    # find our decision, compare evs
                    decision = getattr(row, f"Decision_{i}").strip()
                    our_decision_ev = json_resp.get(decision, 0)

                    resolved_decisions.append(best_decision)
                    evs.append(best_decision_ev - our_decision_ev)
                    print("Our decision: {}, Correct decision: {}, ev diff {}".format(
                        decision, best_decision, best_decision_ev - our_decision_ev))

                df[f"ResolvedDecision_{i}"] = resolved_decisions
                df[f"ResolvedDecision_ev_{i}"] = evs

            df.to_csv('resolved-data/resolved-' + sim_file.strip())
            print("Done {}".format(sim_file))

    print(filename)


if __name__ == '__main__':
    # logging.basicConfig()
    # logging.getLogger().setLevel(logging.DEBUG)
    main()
